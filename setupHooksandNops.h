#pragma once
#include "mem.h"
//EXTERN ALL HOOKS AND NOPS HERE

extern Hook mainHackLoopTramp;
extern NOP infAmmoNOP;
extern NOP infGrenadeNOP;
extern NOP NoRecoilNOP;
extern Hook harvestDataTramp;

void setupHooksandNops();