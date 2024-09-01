#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <GL/gl.h>

#include "aimbot.h"
#include "globals.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// Global Direct3D device and font object
LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr; // Ensure this is set correctly
ID3DXFont* g_pFont = nullptr; // Ensure this is set correctly

// Screen resolution (default values)
const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
const int screenHeight = GetSystemMetrics(SM_CYSCREEN);



// Matrix pointer for view transformation
D3DXMATRIX* pViewMatrix = (D3DXMATRIX*)(0x57DFD0);  // Example address; validate if correct

// Error handling for OpenGL
void CheckGLErrors()
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}

// Set up 2D drawing mode in OpenGL
void SetupGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Convert world coordinates to screen coordinates
bool WorldToScreen(const D3DXVECTOR3& pos, D3DXVECTOR3& screen, int screenWidth, int screenHeight)
{
    if (!pViewMatrix)
    {
        std::cerr << "pViewMatrix is null" << std::endl;
        return false;
    }

    // Transform 3D world coordinates to 4D clip coordinates using the view matrix
    D3DXVECTOR4 clipCoords;
    clipCoords.x = pos.x * pViewMatrix->_11 + pos.y * pViewMatrix->_21 + pos.z * pViewMatrix->_31 + pViewMatrix->_41;
    clipCoords.y = pos.x * pViewMatrix->_12 + pos.y * pViewMatrix->_22 + pos.z * pViewMatrix->_32 + pViewMatrix->_42;
    clipCoords.z = pos.x * pViewMatrix->_13 + pos.y * pViewMatrix->_23 + pos.z * pViewMatrix->_33 + pViewMatrix->_43;
    clipCoords.w = pos.x * pViewMatrix->_14 + pos.y * pViewMatrix->_24 + pos.z * pViewMatrix->_34 + pViewMatrix->_44;

    if (clipCoords.w <= 0.0f)
    {
        std::cerr << "ClipCoords.w is <= 0" << std::endl;
        return false;
    }

    // Perform perspective divide to get normalized device coordinates (NDC)
    D3DXVECTOR3 NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    // Convert NDC to screen coordinates
    screen.x = (screenWidth / 2.0f * NDC.x) + (screenWidth / 2.0f);
    screen.y = -(screenHeight / 2.0f * NDC.y) + (screenHeight / 2.0f);

    if (screen.x < 0.0f || screen.x > screenWidth || screen.y < 0.0f || screen.y > screenHeight)
    {
        return false;
    }

    return true;
}

// Draw the field of view (FOV) on the screen
void DrawFOV()
{
    if (!bEspStatus) return;

    float radius = tanf(D3DXToRadian(bAimbotFOV) / 2) * (screenWidth < screenHeight ? screenWidth : screenHeight);

    glColor3ub(255, 255, 255);
    glLineWidth(1.5f);

    int numSegments = 100;
    float angleStep = 2.0f * gPI / numSegments; // Use M_PI from <cmath>
    float angle = 0.0f;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numSegments; i++)
    {
        float x = (screenWidth / 2.0f) + radius * cosf(angle);
        float y = (screenHeight / 2.0f) + radius * sinf(angle);
        glVertex2f(x, y);
        angle += angleStep;
    }
    glEnd();

    CheckGLErrors();
}

// Draw boxes around players
void DrawBoxes(const Entity& entity)
{
    if (*entity.team != *myself.team)
        glColor3ub(color.red[0], color.red[1], color.red[2]);
    else
        glColor3ub(color.green[0], color.green[1], color.green[2]);

    static const float gameUnits = 450.0f;
    float distanceAdjustment = gameUnits / entity.distanceFromMe;
    float scaleFactor = 3.5f;
    float lengthAdjustment = distanceAdjustment * scaleFactor;
    float widthAdjustment = distanceAdjustment;

    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(entity.screenX - widthAdjustment, screenHeight - (entity.screenY + lengthAdjustment)); // Top left
    glVertex2f(entity.screenX + widthAdjustment, screenHeight - (entity.screenY + lengthAdjustment)); // Top right
    glVertex2f(entity.screenX + widthAdjustment, screenHeight - (entity.screenY - lengthAdjustment)); // Bottom right
    glVertex2f(entity.screenX - widthAdjustment, screenHeight - (entity.screenY - lengthAdjustment)); // Bottom left
    glEnd();

    CheckGLErrors();
}

// Draw everything (ESP)
void DrawEverything()
{
    if (!bEspStatus) return;

    SetupGL();

    for (int i = 0; i < gNumberOfPlayers; i++)
    {
        if (*entity[i].bDead) continue;

        D3DXVECTOR3 screenPos;
        D3DXVECTOR3 worldPos = { *entity[i].xHeadCoord, *entity[i].yHeadCoord, *entity[i].zHeadCoord };

        if (WorldToScreen(worldPos, screenPos, screenWidth, screenHeight))
        {
            entity[i].screenX = screenPos.x;
            entity[i].screenY = screenPos.y;
            DrawBoxes(entity[i]);
        }
    }

    DrawFOV();
}
