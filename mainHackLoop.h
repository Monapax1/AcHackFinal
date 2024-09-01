// mainHackLoop.h
#pragma once
#include <set>
#include <vector>

#include "Esp.h"
// Main hack loop function
void mainHackLoop();
void ProcessEntities(uintptr_t* entList, int numberOfPlayers);
void TrackEntities(const std::vector<BYTE*>& allEntities, const std::set<BYTE*>& currentEntities);
