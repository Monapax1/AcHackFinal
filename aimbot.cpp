#include "pch.h"
#include "aimbot.h"
#include <cmath>
#include <Windows.h>
#include "globals.h"
#include "structs.h"
#include "Esp.h"


// Define the mouse button constant
#define MOUSE_BUTTON_AIM VK_XBUTTON2
int(__fastcall* oIntersectGeometry)(void*, void*) = (int(__fastcall*)(void*, void*))0x4CCA80;

// Function to check if a target is visible
bool IsVisible(const Entity* LocalPlayer, const Entity* Player)
{
    Vec3 TempTo(*Player->xHeadCoord, *Player->yHeadCoord, *Player->zHeadCoord);
    Vec3 from(*LocalPlayer->xHeadCoord, *LocalPlayer->yHeadCoord, *LocalPlayer->zHeadCoord);
    Vec3 to(*Player->xHeadCoord, *Player->yHeadCoord, *Player->zHeadCoord);

    oIntersectGeometry((void*)&from, (void*)&to);

    return to.x == TempTo.x && to.y == TempTo.y && to.z == TempTo.z;
}

// Function to get the closest enemy
int ClosestEnemy()
{
    int closestEnemy = -1;
    float closestDistance = 99999.0f;

    for (int i = 0; i < gNumberOfPlayers; i++)
    {
        if (!(*entity[i].bDead)
            && (*entity[i].team != *myself.team)
            && (IsVisible(&myself, &entity[i]) || !isVisible)
            && (entity[i].distanceFromMe < closestDistance))
        {
            closestDistance = entity[i].distanceFromMe;
            closestEnemy = i;
        }
    }
    return closestEnemy;
}

float NormalizeAngle(float angle)
{
    while (angle > 180.0f) angle -= 360.0f;
    while (angle < -180.0f) angle += 360.0f;
    return angle;
}


// Function to check if the target is within the aimbot's field of view and visible
bool IsInFOV(const Entity& target, const Entity& myself)
{
    // Calculate the differences in coordinates
    float deltaX = *target.xHeadCoord - *myself.xHeadCoord;
    float deltaY = *target.yHeadCoord - *myself.yHeadCoord;
    float deltaZ = *target.zHeadCoord - *myself.zHeadCoord;

    // Calculate the horizontal distance
    float horizontalDistance = sqrt(deltaX * deltaX + deltaY * deltaY);

    // Calculate the target yaw and pitch
    float yawToTarget = atan2(deltaY, deltaX) * 180 / gPI;
    float pitchToTarget = atan2(deltaZ, horizontalDistance) * 180 / gPI;

    // Calculate the difference between the current angles and the target angles
    float relativeYaw = yawToTarget - *myself.yaw;
    float relativePitch = pitchToTarget - *myself.pitch;

    // Normalize the angles to the range [-180, 180]
    relativeYaw = NormalizeAngle(relativeYaw);
    relativePitch = NormalizeAngle(relativePitch);

    // Check if both yaw and pitch are within the specified FOV
    return abs(relativeYaw) <= bAimbotFOV && abs(relativePitch) <= bAimbotFOV;
}


// Function to perform the aimbot action
void Aimbot(int lockOnTarget)
{
    if (!bAimbotStatus || lockOnTarget < 0)
    {
        return;
    }

    float tempYaw = atan2(entity[lockOnTarget].relativeY, entity[lockOnTarget].relativeX) * 180 / gPI + 90;
    float tempPitch = atan2(entity[lockOnTarget].relativeZ, entity[lockOnTarget].distanceFromMe) * 180 / gPI;

    float currentYaw = *myself.yaw;
    float currentPitch = *myself.pitch;

    float yawDifference = tempYaw - currentYaw;
    float pitchDifference = tempPitch - currentPitch;

    if (yawDifference > 180) yawDifference -= 360;
    if (yawDifference < -180) yawDifference += 360;
    if (pitchDifference > 180) pitchDifference -= 360;
    if (pitchDifference < -180) pitchDifference += 360;

    *myself.yaw = currentYaw + yawDifference * bAimbotSmoothing;
    *myself.pitch = currentPitch + pitchDifference * bAimbotSmoothing;
}

// Main update function for aimbot
void UpdateAimbot()
{
    if (GetAsyncKeyState(MOUSE_BUTTON_AIM) & 0x8000)
    {
        int target = ClosestEnemy();
        if (target < 0)
        {
            return; // No valid target found, exit function early
        }

        bool visible = IsVisible(&myself, &entity[target]); // Check if the target is visible
        bool inFOV = IsInFOV(entity[target], myself); // Check if the target is within FOV

        if (!bAimbotFOVEnabled && !isVisible)
        {
            // If both FOV and visibility checks are disabled, aim regardless
            Aimbot(target);
        }
        else if (bAimbotFOVEnabled && !isVisible)
        {
            // If FOV is enabled and visibility check is disabled, aim if in FOV
            if (inFOV)
            {
                Aimbot(target);
            }
        }
        else if (!bAimbotFOVEnabled && isVisible)
        {
            // If FOV check is disabled and visibility check is enabled, aim if visible
            if (visible)
            {
                Aimbot(target);
            }
        }
        else // if (bAimbotFOVEnabled && isVisible)
        {
            // If both FOV and visibility checks are enabled, aim if target is both in FOV and visible
            if (inFOV && visible)
            {
                Aimbot(target);
            }
        }
    }
}

