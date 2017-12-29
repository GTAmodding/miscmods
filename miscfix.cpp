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
	// ignore txd.img
	InjectHook(0x48C12E, 0x48C14C, PATCH_JUMP);

	// increase range of ped/car density
	static float pedMax = 100.0f;
	static float carMax = 100.0f;
	Patch(0x59BE9F + 2, &pedMax);
	Patch(0x59BEB1 + 2, &pedMax);
	Patch(0x59BF16 + 2, &carMax);
	Patch(0x59BF28 + 2, &carMax);
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
