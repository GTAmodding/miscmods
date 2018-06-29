#include "windows.h"
#include <stdint.h>
typedef uintptr_t uintptr;
#include "MemoryMgr.h"	// take from skygfx_vc

typedef uint8_t uint8, uchar;
typedef uint16_t uint16, ushort;
typedef uint32_t uint32, uint;
typedef uint64_t uint64;
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

#define nil NULL
#define FIELD(type, var, offset) *(type*)((uint8*)var + offset)

int gtaversion = -1;

void
patchIII10(void)
{
	// don't set frenchGame and germanGame in
	// CMenuManager::InitialiseChangedLanguageSettings
	InjectHook(0x47A51B, 0x47A537, PATCH_JUMP);

	// and additionally nastyGame, noProstitues and m_PrefsAllowNastyGame in
	// InitialiseLanguage
	InjectHook(0x5821C2, 0x582222, PATCH_JUMP);
}

void
patchVC10(void)
{
	// same as patchIII10
	InjectHook(0x4A38EC, 0x4A3948, PATCH_JUMP);
	InjectHook(0x600B66, 0x600BA2, PATCH_JUMP);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		AddressByVersion<uint32_t>(0, 0, 0, 0, 0, 0);
		if (gtaversion == III_10)
			patchIII10();
		else if(gtaversion == VC_10)
			patchVC10();
	}

	return TRUE;
}
