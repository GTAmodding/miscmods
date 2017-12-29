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


struct CDirectory
{
	struct DirectoryInfo {
		uint32 offset;
		uint32 size;
		char name[24];
	};
	DirectoryInfo *entries;
	int32 maxFiles;
	int32 numFiles;

	void AddItem(DirectoryInfo *dirinfo, int imgid);
	int FindItem(char *name);
};
int
CDirectory::FindItem(char *name)
{
	int i;
	for(i = 0; i < numFiles; i++)
		if(strcmpi(entries[i].name, name) == 0)
			return i;
	return -1;
}
void
CDirectory::AddItem(DirectoryInfo *dirinfo, int imgid)
{
	int i;
	DirectoryInfo dirinfo2 = *dirinfo;
	dirinfo2.offset |= imgid << 24;
	i = FindItem(dirinfo->name);
	if(i < 0)
		entries[numFiles++] = dirinfo2;
}
void __declspec(naked)
dirEntryHookVC(void)
{
	_asm{
		mov     edx, [esp+40h + 0x8]	// fileID
		lea	eax, [esp+40h - 0x34]	// dir entry
		mov	ecx, ds:0xA10730	// CStreaming::ms_pExtraObjectsDir
		push	edx
		push	eax
		call	CDirectory::AddItem

		push	dword ptr 0x40FCA2
		retn
	}
}

void __declspec(naked)
dirEntryHookIII(void)
{
	_asm{
		mov     edx, [esp+48h + 0x8]	// fileID
		lea	eax, [esp+48h - 0x3C]	// dir entry
		mov	ecx, ds:0x95CB90	// CStreaming::ms_pExtraObjectsDir
		push	edx
		push	eax
		call	CDirectory::AddItem

		push	dword ptr 0x406F30
		retn
	}
}


void
patchVC10(void)
{
	InjectHook(0x40FC92, dirEntryHookVC, PATCH_JUMP);
}

void
patchIII10(void)
{
	InjectHook(0x406F20, dirEntryHookIII, PATCH_JUMP);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		AddressByVersion<uint32_t>(0, 0, 0, 0, 0, 0);
		if (gtaversion == VC_10)
			patchVC10();
		else if (gtaversion == III_10)
			patchIII10();
	}

	return TRUE;
}
