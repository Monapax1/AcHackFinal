#pragma once
#include "Esp.h"
#include "structs.h"

// Function declarations
int ClosestEnemy();
void Aimbot(int lockOnTarget);


// Typedef and assignment for IntersectGeometry function
typedef int(__fastcall* IntersectGeometry_t)(void* from, void* to);
extern int(__fastcall* oIntersectGeometry)(void*, void*);// Extern declaration for the function pointer
