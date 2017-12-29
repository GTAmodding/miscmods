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

struct MenuEntry
{
	int32 function;
	char name[8];
	int32 parameter;
	int32 targetmenu;
};
struct MenuScreen
{
  char name[8];
  int32 unk1;
  int32 parent1;
  int32 parent2;
  int32 unk2;
  int32 unk3;
  MenuEntry entries[18];
};
MenuScreen *optionsScreen = (MenuScreen*)0x615754;

struct GlobalScene
{
	void *world;
	void *camera;
};
GlobalScene &Scene = *(GlobalScene*)0x726768;

void
patchIII10(void)
{
	// remove player skin setup
	optionsScreen->entries[4] = optionsScreen->entries[5];
	memset(&optionsScreen->entries[5], 0, sizeof(MenuEntry));
	// don't init skin
	Nop(0x48C09A, 5);
	struct{uchar m[3];} m = { {0xc2, 0x04, 0x00} };
	Patch(0x4A16D0, m);
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
