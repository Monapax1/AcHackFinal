#include "mainHackLoop.h"
#include "aimbot.h"
#include "consoleAndInput.h"
#include "Esp.h"
#include "globals.h"
#include "harvest.h"
#include "setupHooksandNops.h"
#include "structs.h"
#include <vector>
#include <set>
#include "mem.h"
#include <iostream>
#include <thread>

// Constants for offsets
constexpr uintptr_t numberOfPlayersAddress = 0x18AC0C;
constexpr uintptr_t entListOffset = 0x18AC04;
constexpr uintptr_t localPlayerOffset = 0x18AC00; // Fixed offset to match player base
constexpr int entStride = 4;

Entity* closestPlayer = nullptr;
float closestDistance = 9999.0f;

// Function to process entities
void ProcessEntities(uintptr_t* entList, int numberOfPlayers) {
    std::vector<BYTE*> allEntities;
    std::set<BYTE*> currentEntities;
    BYTE* localPlayerBase = nullptr;

    // Retrieve local player base address
    uintptr_t localPlayerAddress = *reinterpret_cast<uintptr_t*>(gModuleBaseAssaultCube + localPlayerOffset);
    if (localPlayerAddress != 0) {
        localPlayerBase = reinterpret_cast<BYTE*>(localPlayerAddress);
    }
    else {
        std::cerr << "Failed to get local player address. Address: " << localPlayerOffset << std::endl;
        return;
    }


    for (int i = 4; i < numberOfPlayers; i++) {
        uintptr_t ent = *reinterpret_cast<uintptr_t*>(*entList + (i * entStride));
        if (ent) {
            BYTE* entBase = reinterpret_cast<BYTE*>(ent);
            int* entNumber = reinterpret_cast<int*>(entBase + offsets.playerNumber);
            MoveDataIntoEnt(entBase, entNumber);
            allEntities.push_back(entBase);
            currentEntities.insert(entBase);

            if (ent == localPlayerAddress) {
                localPlayerBase = entBase;
            }
        }
        else {
            std::cerr << "Entity at index " << i << " is invalid. Address: " << ent << std::endl;
        }
    }

    if (localPlayerBase) {
        int* entNumber = reinterpret_cast<int*>(localPlayerBase + offsets.playerNumber);
        MoveDataIntoEnt(localPlayerBase, entNumber);
        TrackEntities(allEntities, currentEntities);
        allEntities.push_back(localPlayerBase);
        currentEntities.insert(localPlayerBase);
    }
    else {
        std::cerr << "Local player base address is not set correctly. Address: " << localPlayerBase << std::endl;
    }
}

// Function to track entities
void TrackEntities(const std::vector<BYTE*>& allEntities, const std::set<BYTE*>& currentEntities) {
    std::set<BYTE*> previousEntities(allEntities.begin(), allEntities.end());
    for (BYTE* entBase : previousEntities) {
        if (currentEntities.find(entBase) == currentEntities.end()) {
            int* entNumber = reinterpret_cast<int*>(entBase + offsets.playerNumber);
            if (entNumber) {
                int index = *entNumber;
                if (index >= 0 && index < gNumberOfPlayers) {
                    entity[index].bChecked = false;
                }
                else {
                    std::cerr << "Invalid entity index: " << index << std::endl;
                }
            }
        }
    }
}

// Main hack loop function
void mainHackLoop() {
    int* numberOfPlayersPtr = reinterpret_cast<int*>(gModuleBaseAssaultCube + numberOfPlayersAddress);
    if (!numberOfPlayersPtr) {
        std::cerr << "Failed to retrieve number of players address. Address: " << numberOfPlayersAddress << std::endl;
        return;
    }

    int currentNumberOfPlayers = *numberOfPlayersPtr;

    if (gNumberOfPlayers != currentNumberOfPlayers) {
        gNumberOfPlayers = currentNumberOfPlayers;
    }

    uintptr_t* entList = reinterpret_cast<uintptr_t*>(gModuleBaseAssaultCube + entListOffset);
    if (!entList || !*entList) {
        std::cerr << "Entity list is invalid. Address: " << entListOffset << std::endl;
        return;
    }

    if (harvestDataTramp.bActive) return;

    ProcessEntities(entList, gNumberOfPlayers);
    UpdateEntInfo();

    if (bAimbotStatus) {
        int target = ClosestEnemy();
        if (target != -1) {
            Aimbot(target);
        }
    }
    GetInput();
    DrawEverything();

}
