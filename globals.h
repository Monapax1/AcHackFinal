#pragma once

#include <atomic>
#include <GLFW/glfw3.h>

#include "Esp.h"

// External variables
extern BYTE* gModuleBaseAssaultCube;
extern BYTE* gModuleBaseOpenGl;
extern int gNumberOfPlayers;
extern float gPI;
extern bool bAimbotStatus;
extern bool bEspStatus;
extern float bAimbotSmoothing;
extern float bAimbotFOV;
extern bool isVisible;
extern int viewport[4];            // Viewport dimensions and offsets (x, y, width, height)
extern bool bEspStatus;            // ESP status flag (e.g., whether ESP is enabled)
extern bool bAimbotFOVEnabled;