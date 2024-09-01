#pragma once
#include <Windows.h>

struct Hook
{
    // Get to our hook
    BYTE* hookPosition{ nullptr };
    // Store stolen bytes
    BYTE stolenBytes[20]{ 0 };
    int lengthOfHook{ 0 };
    // Is hook active?
    bool bActive{ false };

    // Trampoline hook setup
    BYTE* desiredFunction{ nullptr };
    DWORD hookToTrampJump{ 0 };  // Jump at the hook to our trampoline
    DWORD trampToFuncCall{ 0 };  // Call our function from trampoline
    DWORD returnJump{ 0 };       // Jump at the end of our trampoline back to the flow of execution
    // Trampoline placement 
    BYTE* trampoline{ nullptr };
    // Don't corrupt stack
    BYTE preserveStack[2]{ 0x60, 0x9C };  // pushad, pushfd
    BYTE releaseStack[2]{ 0x9D, 0x61 };   // popfd, popad
    bool bTrampBuild{ false };

    // Detour setup
    DWORD hookToDetourJump{ 0 };
    DWORD returnJumpDetour{ 0 };

    // Functionality
    void ToggleTrampSBF();
    void ToggleDetour();

    // Constructors
    Hook(BYTE* hookPosition, BYTE* desiredFunction, int lengthOfHook);
    Hook() = default;
};

struct NOP
{
    // Where at and for how long?
    BYTE* nopPosition{ nullptr };
    int length{ 0 };
    // Save for toggling off
    BYTE stolenBytes[30]{ 0 };
    // Used by function to know whether to store original bytes, Nop, or replace nopPosition with stolenBytes
    bool bActive{ false };

    // Functionality
    void ToggleNOP();

    // Constructors
    NOP(BYTE* NOPPosition, int length);
    NOP() = default;
};
