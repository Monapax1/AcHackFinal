#include "pch.h"
#include "harvest.h"
#include "Esp.h"
#include "structs.h"
#include "setupHooksandNops.h"
#include "globals.h"
#include <iostream>
#include <thread>
#include "aimbot.h"
#include "consoleAndInput.h"

// Function prototypes
void MoveDataIntoEnt(BYTE* playerBase, int* playerNumber);

void HarvestData()
{
    int* currentPlayerNumber = nullptr;
    BYTE* playerBase = nullptr;

    // Inline assembly to obtain playerBase and currentPlayerNumber
    __asm
    {
        push ebx
        lea ebx, [esi]
        mov playerBase, ebx
        lea ebx, [esi + 0x1E4]
        mov currentPlayerNumber, ebx
        pop ebx
    }

    if (currentPlayerNumber && playerBase)
    {
        MoveDataIntoEnt(playerBase, currentPlayerNumber);
    }
    else
    {
        std::cerr << "Failed to get valid player data." << std::endl;
        return;
    }

    // Check if all entities have been processed
    for (int i = 0; i < gNumberOfPlayers; ++i)
    {
        if (!entity[i].bChecked)
        {
            return;
        }
    }

    // Reset the checked status
    for (int i = 0; i < gNumberOfPlayers; ++i)
    {
        entity[i].bChecked = false;
    }

    // Toggle trampoline function
    harvestDataTramp.ToggleTrampSBF();
}

void MoveDataIntoEnt(BYTE* playerBase, int* playerNumber)
{
    if (!playerBase || !playerNumber)
    {
        std::cerr << "Invalid player base or player number." << std::endl;
        return;
    }

    int playerIndex = *playerNumber;

    if (playerIndex < 0 || playerIndex >= gNumberOfPlayers)
    {
        std::cerr << "Invalid player index: " << playerIndex << std::endl;
        return;
    }

    auto& playerEntity = entity[playerIndex];

    // Update entity data
    playerEntity.playerBase = playerBase;
    playerEntity.bDead = playerBase + offsets.bDead;
    playerEntity.playerNumber = reinterpret_cast<int*>(playerBase + offsets.playerNumber);
    playerEntity.team = reinterpret_cast<int*>(playerBase + offsets.team);
    playerEntity.health = reinterpret_cast<int*>(playerBase + offsets.health);
    playerEntity.xOrigin = reinterpret_cast<float*>(playerBase + offsets.xOrigin);
    playerEntity.yOrigin = reinterpret_cast<float*>(playerBase + offsets.yOrigin);
    playerEntity.zOrigin = reinterpret_cast<float*>(playerBase + offsets.zOrigin);
    playerEntity.xHeadCoord = reinterpret_cast<float*>(playerBase + offsets.xHeadCoord);
    playerEntity.yHeadCoord = reinterpret_cast<float*>(playerBase + offsets.yHeadCoord);
    playerEntity.zHeadCoord = reinterpret_cast<float*>(playerBase + offsets.zHeadCoord);
    playerEntity.grenade = reinterpret_cast<int*>(playerBase + offsets.grenade);
    playerEntity.name = reinterpret_cast<char*>(playerBase + offsets.name);
    playerEntity.currentWeapon = reinterpret_cast<Game::GunInfo*>(playerBase + offsets.currentWeapon);
    playerEntity.bChecked = true;

    // Set visibility flag if applicable
    playerEntity.bVisible = isVisible;

    // Update 'myself' if this is player index 0
    if (playerIndex == 0)
    {
        myself.pitch = reinterpret_cast<float*>(playerBase + offsets.pitch);
        myself.yaw = reinterpret_cast<float*>(playerBase + offsets.yaw);
        myself.health = reinterpret_cast<int*>(playerBase + offsets.health);
        myself.Ammo = reinterpret_cast<int*>(playerBase + offsets.Ammo);
        myself.currentWeapon = reinterpret_cast<Game::GunInfo*>(playerBase + offsets.currentWeapon);
        myself.team = reinterpret_cast<int*>(playerBase + offsets.team);
        myself.playerBase = playerBase;
        myself.bDead = playerBase + offsets.bDead;
        myself.xOrigin = reinterpret_cast<float*>(playerBase + offsets.xOrigin);
    	myself.yOrigin = reinterpret_cast<float*>(playerBase + offsets.yOrigin);
        myself.zOrigin = reinterpret_cast<float*>(playerBase + offsets.zOrigin);
        myself.xHeadCoord = reinterpret_cast<float*>(playerBase + offsets.xHeadCoord);
        myself.yHeadCoord = reinterpret_cast<float*>(playerBase + offsets.yHeadCoord);
        myself.zHeadCoord = reinterpret_cast<float*>(playerBase + offsets.zHeadCoord);
        myself.name = reinterpret_cast<char*>(playerBase + offsets.name);
        myself.playerNumber = reinterpret_cast<int*>(playerBase + offsets.playerNumber);
        myself.grenade = reinterpret_cast<int*>(playerBase + offsets.grenade);
        myself.Armor = reinterpret_cast<int*>(playerBase + offsets.grenade);
    }
}
