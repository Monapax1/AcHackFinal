#include "pch.h"
#include "infAmmo.h"
#include "setupHooksandNops.h"

void __declspec(naked)infAmmo()
{
	__asm
	{
		mov eax, [esi + 0x14] // the only stolen Bytes we wanna process
		inc[eax]  //incrementing our ammo
		jmp infAmmoDetour.returnJumpDetour //using our instance s return jump to jump after after the hook
	 }
}