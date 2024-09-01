#include "globals.h"


BYTE* gModuleBaseAssaultCube = (BYTE*)GetModuleHandle(L"ac_client.exe");
BYTE* gModuleBaseOpenGl = (BYTE*)GetModuleHandle(L"opengl32.dll");
int gNumberOfPlayers = *(int*)(gModuleBaseAssaultCube + 0x18AC0C);
const DWORD_PTR gViewMatrixAddress = 0x57DFD0;
float gPI = 3.14159265359f;
bool bAimbotStatus = false;
bool bEspStatus = false;
float bAimbotSmoothing = 1.0f;
float bAimbotFOV = 10.0f;
bool isVisible = false;
bool bAimbotFOVEnabled = false;
