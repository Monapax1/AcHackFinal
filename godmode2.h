// godmode2.h
#pragma once

#include <atomic>

// External declarations
extern std::atomic<bool> godModeEnabled;
extern std::atomic<bool> godModeThreadRunning;
extern std::atomic<bool> autoGunsEnabled;
extern std::atomic<bool> noSpreadEnabled;
extern std::atomic<bool> noWaitEnabled;
extern std::atomic<bool> AddGrenadesEnabled;

// Function declarations
void SetHealth(int newHealth);
void GodModeLoop();
void StartGodModeThread();
int GetCurrentWeaponID();
void SetAllGunsToAuto();
void SetNoSpread();
void SetNoWait();
void ToggleNoSpread();
void ToggleAutoGuns();
void ToggleNoWait();
void ToggleGodMode();
void AddGrenades();
void ToggleAddGrenades();
