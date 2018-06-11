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
	// Taken from gta3d mod
	// feel free to experiment

	//
	// vehicle light flare
	//
	// probably leave these alone
//	Patch<uchar>(0x537CA4+1, 2);
//	Patch<uchar>(0x537DAB+1, 2);
//	Patch<uchar>(0x537F35+1, 2);

	// light 0 1
	Patch<uchar>(0x5384AD+1, 2);
	Patch<uchar>(0x53856C+1, 2);
	Patch<uchar>(0x538511+1, 2);
	Patch<uchar>(0x5385CC+1, 2);
	// kinda paired with bright light; light 0 1 
//	Patch<uchar>(0x538AE5+1, 2);
//	Patch<uchar>(0x538654+1, 2);
//	Patch<uchar>(0x538777+1, 2);
//	Patch<uchar>(0x5388A2+1, 2);
//	Patch<uchar>(0x5389C5+1, 2);
	// kinda paired with bright light; light 2 3
//	Patch<uchar>(0x5391F5+1, 2);
//	Patch<uchar>(0x539244+1, 2);
//	Patch<uchar>(0x5390B4+1, 2);
//	Patch<uchar>(0x539155+1, 2);
//	Patch<uchar>(0x538E31+1, 2);
//	Patch<uchar>(0x538F4C+1, 2);
	// paired with bright light
//	Patch<uchar>(0x539B49+1, 2);
//	Patch<uchar>(0x539B9F+1, 2);
//	// paired with bright light
//	Patch<uchar>(0x5399C5+1, 2);
//	Patch<uchar>(0x539A1B+1, 2);
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
