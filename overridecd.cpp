#include "windows.h"
#include <stdint.h>
typedef uintptr_t uintptr;
#include "MemoryMgr_all.h"	// take from debugmenu

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

struct CdImage
{
	char name[40];
	char isNotPlayerImg;
	HANDLE streamHandle;
};

CdImage *CStreaming__ms_files = (CdImage*)0x8E48D8;
WRAPPER void CStreaming__LoadCdDirectory(const char *name, int fileid) { EAXJMP(0x5B6170); }

void
LoadCdDirectories(void)
{
	int n;
	for(n = 0; n < 8; n++)
		if(CStreaming__ms_files[n].name[0] == '\0')
			break;
	while(n--)
		if(CStreaming__ms_files[n].isNotPlayerImg)
			CStreaming__LoadCdDirectory(CStreaming__ms_files[n].name, n);
}

void
patchSA10(void)
{
	InjectHook(0x5B82F0, LoadCdDirectories, PATCH_CALL);
	InjectHook(0x5B82F0 + 5, 0x5B8324, PATCH_JUMP);

	// swap gta3.img and gta_int.img
	Patch(0x408430 + 1, 0x858B20);
	Patch(0x40844C + 1, 0x858B20);

	Patch(0x40846E + 1, 0x858AF4);
	Patch(0x40848C + 1, 0x858AF4);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		AddressByVersion<uint32_t>(0, 0, 0, 0, 0, 0, 0);
		if (gtaversion == SA_10)
			patchSA10();
	}

	return TRUE;
}
