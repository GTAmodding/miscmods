#include "windows.h"
#include <stdint.h>
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

int gtaversion = -1;

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	uintptr addr;
	if(reason == DLL_PROCESS_ATTACH){
		addr = AddressByVersion<uint32_t>(0x48C975, 0, 0, 0x4A45C3, 0, 0, 0x53C090);
		if(addr)
			Nop(addr, 5);
	}

	return TRUE;
}
