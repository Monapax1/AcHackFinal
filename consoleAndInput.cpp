#include "consoleAndInput.h"
#include <algorithm>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include "aimbot.h"
#include "Esp.h"
#include "globals.h"
#include "godmode2.h"
#include "harvest.h"
#include "setupHooksandNops.h"
#include "structs.h" // Include structs.h to access features functions

bool bBreakHackThreadWhileLoop = false;

// Global flags to control features
extern std::atomic<bool> godModeEnabled;
extern std::atomic<bool> godModeThreadRunning;
extern std::atomic<bool> autoGunsEnabled;
extern std::atomic<bool> noSpreadEnabled;
extern std::atomic<bool> noWaitEnabled;
extern std::atomic<bool> AddGrenadesEnabled;

// Function to handle user input and toggle cheats
void GetInput()
{
    // Handle keypresses for toggling cheats
    if (GetAsyncKeyState(VK_END) & 1)
    {
        BreakHackLoop();
    }

    if (GetAsyncKeyState(VK_NUMPAD0) & 1)
    {
        infAmmoNOP.ToggleNOP();
        PrintConsole();
    }

    if (GetAsyncKeyState(VK_NUMPAD1) & 1) // Check if NUMPAD1 is pressed
    {
        godModeEnabled.store(!godModeEnabled.load()); // Toggle god mode
        if (godModeEnabled.load())
        {
            if (!godModeThreadRunning.load())
            {
                StartGodModeThread(); // Start the god mode thread if it is enabled
            }
        }
        else
        {
            godModeThreadRunning.store(false); // Signal to stop the god mode loop
        }
        PrintConsole(); // Print console to reflect the change
        Sleep(200); // Delay to prevent multiple toggles from a single press
    }

    if (GetAsyncKeyState(VK_NUMPAD2) & 1)
    {
        infGrenadeNOP.ToggleNOP();
        PrintConsole();
    }

    if (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) // Mouse Button 5 is held down
    {
        bAimbotStatus = true;
    }
    else
    {
        bAimbotStatus = false;
    }

    if (GetAsyncKeyState(VK_NUMPAD5) & 1)
    {
        bEspStatus = !bEspStatus;
        PrintConsole();
    }

    // Increase aimbot smoothing
    if (GetAsyncKeyState(VK_ADD) & 1)
    {
        bAimbotSmoothing = min(1.0f, bAimbotSmoothing + 0.1f);  // Increase smoothing factor
        PrintConsole();
    }

    // Decrease aimbot smoothing
    if (GetAsyncKeyState(VK_SUBTRACT) & 1)
    {
        bAimbotSmoothing = max(0.0f, bAimbotSmoothing - 0.1f);  // Decrease smoothing factor
        PrintConsole();
    }

    // Increase aimbot FOV
    if (GetAsyncKeyState(VK_NUMPAD7) & 1)
    {
        bAimbotFOV = min(100.0f, bAimbotFOV + 0.1f); // Increase FOV
        PrintConsole();
    }

    // Decrease aimbot FOV
    if (GetAsyncKeyState(VK_NUMPAD4) & 1)
    {
        bAimbotFOV = max(0.0f, bAimbotFOV - 0.1f); // Decrease FOV
        PrintConsole();
    }

    if (GetAsyncKeyState(VK_NUMPAD8) & 1)
    {
        NoRecoilNOP.ToggleNOP();
        PrintConsole();
    }

    if (GetAsyncKeyState(VK_NUMPAD9) & 1)
    {
        isVisible = !isVisible; // Toggle visibility check
        PrintConsole(); // Print console to reflect change
    }

    if (GetAsyncKeyState(VK_F1) & 1)
    {
        noSpreadEnabled.store(!noSpreadEnabled.load()); // Toggle no spread
        ToggleNoSpread();
        PrintConsole();
    }

    if (GetAsyncKeyState(VK_F2) & 1)
    {
        autoGunsEnabled.store(!autoGunsEnabled.load()); // Toggle auto guns
        ToggleAutoGuns();
        PrintConsole();
    }

    if (GetAsyncKeyState(VK_F3) & 1)
    {
        noWaitEnabled.store(!noWaitEnabled.load()); // Toggle no wait
        ToggleNoWait();
        PrintConsole();
    }

    if (GetAsyncKeyState(VK_NUMPAD3) & 1)
    {
        AddGrenadesEnabled.store(!AddGrenadesEnabled.load());
        ToggleAddGrenades();
        PrintConsole();
    }
}

// Print console with current cheat statuses
void PrintConsole()
{
    SetConsoleTitle(L"21c Assault Cube Cheeto");
    system("CLS");

    std::cout << "Press NUMPAD0 for infAmmoNOP" << std::endl;
    std::cout << "Press NUMPAD1 for GodMode" << std::endl;
    std::cout << "Press NUMPAD2 for Inf Grenades/ Have at least 1" << std::endl;
    std::cout << "Press NUMPAD+ for more Smoothing!" << std::endl;
    std::cout << "Press NUMPAD- for less Smoothing!" << std::endl;
    std::cout << "Press MouseButton2 for AimLock!" << std::endl;
    std::cout << "Press NUMPAD5 for ESP!" << std::endl;
    std::cout << "Press NUMPAD7 for more FOV!" << std::endl;
    std::cout << "Press NUMPAD4 for less FOV!" << std::endl;
    std::cout << "Press NUMPAD8 for No Recoil!" << std::endl;
    std::cout << "Press NUMPAD9 to toggle Visibility Check!" << std::endl;
    std::cout << "Press F1 to toggle No Spread!" << std::endl;
    std::cout << "Press F2 to toggle Auto Guns!" << std::endl;
    std::cout << "Press F3 to toggle No Wait!" << std::endl;
    std::cout << "Press END to unInject" << std::endl;
    std::cout << "====================================================" << std::endl;

    std::cout << (infAmmoNOP.bActive ? "infAmmoNOP: On\n" : "infAmmoNOP: Off\n");
    std::cout << (godModeEnabled.load() ? "GodMode: On\n" : "GodMode: Off\n");
    std::cout << (infGrenadeNOP.bActive ? "Inf Grenades: On\n" : "Inf Grenades: Off\n");
    std::cout << (NoRecoilNOP.bActive ? "NoRecoil: On\n" : "NoRecoil: Off\n");
    std::cout << "Aimbot: " << (bAimbotStatus ? "On\n" : "Off\n");
    std::cout << "ESP: " << (bEspStatus ? "On\n" : "Off\n");
    std::cout << "Aimbot Smoothing: " << bAimbotSmoothing << std::endl;
    std::cout << "Aimbot FOV: " << bAimbotFOV << std::endl;
    std::cout << "Visibility Check: " << (isVisible ? "On\n" : "Off\n"); // Print visibility check status
    std::cout << "No Spread: " << (noSpreadEnabled.load() ? "On\n" : "Off\n"); // Print no spread status
    std::cout << "Auto Guns: " << (autoGunsEnabled.load() ? "On\n" : "Off\n"); // Print auto guns status
    std::cout << "No Wait: " << (noWaitEnabled.load() ? "On\n" : "Off\n"); // Print no wait status
}

// Break the hack loop and deactivate all cheats
void BreakHackLoop()
{
    // Deactivate all cheats and hooks
    if (infAmmoNOP.bActive) infAmmoNOP.ToggleNOP();
    if (infGrenadeNOP.bActive) infGrenadeNOP.ToggleNOP();
    if (harvestDataTramp.bActive) harvestDataTramp.ToggleTrampSBF();
    if (NoRecoilNOP.bActive) NoRecoilNOP.ToggleNOP();
    if (bAimbotStatus) bAimbotStatus = false;
    if (bEspStatus) bEspStatus = false;
    if (isVisible) isVisible = false;
    if (godModeEnabled.load()) godModeEnabled.store(false);
    if (autoGunsEnabled.load()) autoGunsEnabled.store(false);
    if (noSpreadEnabled.load()) noSpreadEnabled.store(false);
    if (noWaitEnabled.load()) noWaitEnabled.store(false);

    // Toggle off main hook loop, then break while loop inside the HackThread()
    mainHackLoopTramp.ToggleTrampSBF();
    godModeThreadRunning.store(false); // Signal to stop the god mode loop
    bBreakHackThreadWhileLoop = true;
}

// Toggle console window
void ToggleConsole()
{
    static bool bActive = false;
    static FILE* f = nullptr;
    bActive = !bActive;

    if (bActive)
    {
        AllocConsole();
        freopen_s(&f, "CONOUT$", "w", stdout);
        freopen_s(&f, "CONIN$", "r", stdin);
        SetConsoleTitle(L"Console Window");
        std::cout << "Console is now active." << std::endl;
    }
    else
    {
        fclose(stdout);
        fclose(stdin);
        FreeConsole();
    }
}
