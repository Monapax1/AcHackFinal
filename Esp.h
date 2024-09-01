#pragma once

#include <d3dx9math.h>  // For DirectX math operations

#include "structs.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <GL/GL.h>
// For necessary type definitions, e.g., Entity

// Function declarations
void SetupGL();                    // Initializes OpenGL (or DirectX) settings
void DrawEverything();             // Draws all necessary elements
void DrawBoxes(const Entity& entity);    // Draws bounding boxes around entities
bool WorldToScreen(const D3DXVECTOR3& pos, D3DXVECTOR3& screen, const D3DXMATRIX& matrix, int width, int height); // Converts world coordinates to screen coordinates

