#pragma once

#include <iostream>
#include <thread>
#include <Windows.h>  // Necessary if using Windows-specific APIs

#include "structs.h"  // Ensure this file is correctly included for struct definitions

// Function declarations
void HarvestData();
void MoveDataIntoEnt(BYTE* playerBase, int* playerNumber);

// Define the harvest class if needed, or remove it if not used
class Harvest
{
};
