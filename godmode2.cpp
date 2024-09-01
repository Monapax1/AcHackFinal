#include "godmode2.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <Windows.h>
#include "globals.h"
#include "structs.h"

// External declarations
std::atomic<bool> godModeEnabled(false);
std::atomic<bool> godModeThreadRunning(false);
std::atomic<bool> autoGunsEnabled(false);
std::atomic<bool> noSpreadEnabled(false);
std::atomic<bool> noWaitEnabled(false);
std::atomic<bool> AddGrenadesEnabled(false);

bool ReadMemory(HANDLE processHandle, uintptr_t address, void* buffer, size_t size)
{
    SIZE_T bytesRead;
    return ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), buffer, size, &bytesRead) && bytesRead == size;
}

bool WriteMemory(HANDLE processHandle, uintptr_t address, const void* buffer, size_t size)
{
    SIZE_T bytesWritten;
    return WriteProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), buffer, size, &bytesWritten) && bytesWritten == size;
}

void SetHealth(int newHealth)
{
    if (myself.health != nullptr)
    {
        *myself.health = newHealth;
    }
    else
    {
        std::cerr << "Error: Health pointer is null." << std::endl;
    }
}

void GodModeLoop()
{
    godModeThreadRunning.store(true);

    while (godModeThreadRunning.load())
    {
        if (godModeEnabled.load())
        {
            SetHealth(100);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

void StartGodModeThread()
{
    static std::thread godModeThread;
    if (godModeThread.joinable())
    {
        godModeThreadRunning.store(false);
        godModeThread.join();
    }

    godModeThread = std::thread(GodModeLoop);
    godModeThread.detach();
}

void SetAllGunsToAuto()
{

    myself.currentWeapon->bIsAuto = true;
}
void SetNoSpread()
{
    // Set the 'spread' field to 0 for the current weapon
    myself.currentWeapon->spread = 0;
    myself.currentWeapon->mdl_kick_back = 0;
}

void SetNoWait()
{

    // Set the 'attackdelay' and 'reloadtime' fields to 0 for the current weapon
    myself.currentWeapon->attackdelay = 0;
    myself.currentWeapon->reloadtime = 0;

}

void AddGrenades()
{
    *myself.grenade = 100;
}


void ToggleNoSpread()
{
    if (noSpreadEnabled.load())
    {
        SetNoSpread();
    }
}

void ToggleAddGrenades()
{
	if (AddGrenadesEnabled.load())
	{
        AddGrenades();
	}

}
void ToggleAutoGuns()
{
    if (autoGunsEnabled.load())
    {
        SetAllGunsToAuto();
    }
}

void ToggleNoWait()
{
    if (noWaitEnabled.load())
    {
        SetNoWait();
    }
}

void ToggleGodMode()
{
    bool enabled = !godModeEnabled.load();
    godModeEnabled.store(enabled);

    if (enabled)
    {
        StartGodModeThread();
    }
    else
    {
        godModeThreadRunning.store(false);
    }
}
