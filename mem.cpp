#include "pch.h"
#include "mem.h"
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

// Hook constructor
Hook::Hook(BYTE* hookPosition, BYTE* desiredFunction, int lengthOfHook)
    : hookPosition(hookPosition), desiredFunction(desiredFunction), lengthOfHook(lengthOfHook), bTrampBuild(false), bActive(false)
{
    // Copy the original bytes at the hook position
    memcpy(stolenBytes, hookPosition, lengthOfHook);

    // Allocate memory for trampoline
    trampoline = (BYTE*)VirtualAlloc(nullptr, lengthOfHook + sizeof(preserveStack) + 5 + sizeof(releaseStack), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!trampoline) {
        std::cerr << "Failed to allocate memory for trampoline." << std::endl;
        return;
    }

    // Calculate jump addresses for trampoline
    hookToTrampJump = (DWORD)(trampoline - hookPosition) - 5;
    returnJump = (DWORD)(hookPosition + lengthOfHook) - (DWORD)(trampoline + lengthOfHook + sizeof(preserveStack) + 5 + sizeof(releaseStack)) - 5;

    // Calculate detour jump address
    hookToDetourJump = (DWORD)(desiredFunction - hookPosition) - 5;
    returnJumpDetour = (DWORD)(hookPosition + lengthOfHook);
}

// Toggle trampoline hook
void Hook::ToggleTrampSBF()
{
    bActive = !bActive; // Toggle hook state

    DWORD oldProtection;
    VirtualProtect(hookPosition, lengthOfHook, PAGE_EXECUTE_READWRITE, &oldProtection);

    if (bActive)
    {
        if (!bTrampBuild)
        {
            // Set up trampoline function
            trampToFuncCall = (DWORD)desiredFunction - (DWORD)(trampoline + lengthOfHook + sizeof(preserveStack)) - 5;
            memcpy(trampoline, hookPosition, lengthOfHook);
            memcpy(trampoline + lengthOfHook, preserveStack, sizeof(preserveStack));
            *(BYTE*)(trampoline + lengthOfHook + sizeof(preserveStack)) = 0xE8; // CALL instruction
            *(DWORD*)(trampoline + lengthOfHook + sizeof(preserveStack) + 1) = trampToFuncCall;
            memcpy(trampoline + lengthOfHook + sizeof(preserveStack) + 5, releaseStack, sizeof(releaseStack));
            *(BYTE*)(trampoline + lengthOfHook + sizeof(preserveStack) + 5 + sizeof(releaseStack)) = 0xE9; // JMP instruction
            *(DWORD*)(trampoline + lengthOfHook + sizeof(preserveStack) + 5 + sizeof(releaseStack) + 1) = returnJump;
            bTrampBuild = true; // Mark trampoline as built
        }

        // Set hook in the game's memory
        memset(hookPosition, 0x90, lengthOfHook); // NOP instructions
        *(BYTE*)hookPosition = 0xE9; // JMP instruction
        *(DWORD*)(hookPosition + 1) = hookToTrampJump;
    }
    else
    {
        // Restore original bytes
        memcpy(hookPosition, stolenBytes, lengthOfHook);
    }

    VirtualProtect(hookPosition, lengthOfHook, oldProtection, nullptr); // Restore original protection
}

// Toggle detour hook
void Hook::ToggleDetour()
{
    bActive = !bActive; // Toggle hook state

    DWORD oldProtection;
    VirtualProtect(hookPosition, lengthOfHook, PAGE_EXECUTE_READWRITE, &oldProtection);

    if (bActive)
    {
        memset(hookPosition, 0x90, lengthOfHook); // NOP instructions
        *(BYTE*)hookPosition = 0xE9; // JMP instruction
        *(DWORD*)(hookPosition + 1) = hookToDetourJump;
    }
    else
    {
        memcpy(hookPosition, stolenBytes, lengthOfHook); // Restore original bytes
    }

    VirtualProtect(hookPosition, lengthOfHook, oldProtection, nullptr); // Restore original protection
}

// NOP constructor
NOP::NOP(BYTE* nopPosition, int length)
    : nopPosition(nopPosition), length(length), bActive(false)
{
    memcpy(stolenBytes, nopPosition, length);
}

// Toggle NOP operation
void NOP::ToggleNOP()
{
    bActive = !bActive; // Toggle NOP state

    DWORD oldProtection;
    VirtualProtect(nopPosition, length, PAGE_EXECUTE_READWRITE, &oldProtection);

    if (bActive)
    {
        memset(nopPosition, 0x90, length); // NOP instructions
    }
    else
    {
        memcpy(nopPosition, stolenBytes, length); // Restore original bytes
    }

    VirtualProtect(nopPosition, length, oldProtection, nullptr); // Restore original protection
}

// Get process ID by name
DWORD GetProcessID(const std::wstring& processName)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return 0;

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &processEntry))
    {
        do
        {
            if (processName == processEntry.szExeFile)
            {
                CloseHandle(snapshot);
                return processEntry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return 0;
}
