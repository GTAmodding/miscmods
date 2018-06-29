#define _USE_MATH_DEFINES
#pragma comment(lib, "user32")

#include "windows.h"
#include <stdint.h>
#include <cmath>
typedef uintptr_t uintptr;
#include "MemoryMgr_all.h"

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

#define DEGTORAD(x) (float)((x)/180.0f*M_PI)


int gtaversion = -1;

static float returnAngle1(float x, float y) { return DEGTORAD(1.3f); }
static float returnAngle2(float x, float y) { return DEGTORAD(0.0f); }
static float returnAngle3(float x, float y) { return DEGTORAD(10.8f); }

void
patchIII10(void)
{
	// beta-stlye vehicle cam
	Patch(0x468953 + 6, 0.05f);
	Patch(0x468972 + 6, 1.6f);
	Patch(0x468992 + 6, 2.4f);
	static float heightoffset = 0.35f;
	Patch(0x45C1D8 + 2, &heightoffset);

	InjectHook(0x466769, returnAngle1);
	InjectHook(0x4667A0, returnAngle2);
	InjectHook(0x4667D4, returnAngle3);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		if(AddressByVersion<uint32_t>(1, 0, 0, 0, 0, 0, 0))
			patchIII10();
	}

	return TRUE;
}
