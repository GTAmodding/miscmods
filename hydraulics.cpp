#define _USE_MATH_DEFINES
#pragma comment(lib, "user32")

#include "windows.h"
#include <stdint.h>
#include <cmath>
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

void __declspec(naked)
checkHydraulics(void)
{
	_asm{
		cmp     eax, 135	// yardie
		je	hydraulics
		cmp     eax, 109	// esperanto
		je	hydraulics
		// no hydraulics
		push	0x532035
		retn
	hydraulics:
		push	0x532027
		retn
	}
}

void
patchIII10(void)
{
	// hydraulics for esperanto
	InjectHook(0x532020, checkHydraulics, PATCH_JUMP);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		AddressByVersion<uint32_t>(0, 0, 0, 0, 0, 0);
		if (gtaversion == III_10)
			patchIII10();
	}

	return TRUE;
}
