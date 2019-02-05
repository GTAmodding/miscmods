#pragma comment(lib, "user32")

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdint.h>
typedef uintptr_t uintptr;
#include "MemoryMgr_all.h"

int gtaversion = -1;
char userPath[MAX_PATH];
void *&Camera = *AddressByVersion<void**>(0x72676C, 0x72676C, 0x7368AC, 0x8100BC, 0x8100C4, 0x80F0C4, 0);
char *tracksPath = (char*)0xC92168;
char *galleryPath = (char*)0xC92268;

void
makedir(const char *path)
{
	HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING|FILE_ATTRIBUTE_NORMAL, NULL);
	if(h == (HANDLE)-1)
		CreateDirectoryA(path, NULL);
	else
		CloseHandle(h);
}

char*
getUserFilesDir(void)
{
	makedir(userPath);
	if(gtaversion == SA_10){
		strcpy(galleryPath, userPath);
		strcat(galleryPath, "\\Gallery");
		makedir(galleryPath);

		strcpy(tracksPath, userPath);
		strcat(tracksPath, "\\User Tracks");
		makedir(tracksPath);
	}
	return userPath;
}

void (*getSaveFileName_orig)(const char *dir);
void
getSaveFileName(const char *dir)
{
	getSaveFileName_orig(getUserFilesDir());
}

int (*RsEventHandler_orig)(int a, int b);
int
delayedPatches(int a, int b)
{
	char *p;
	GetModuleFileName(NULL, userPath, MAX_PATH);
	p = strrchr(userPath, '\\');
	strcpy(p+1, "userfiles");

	// Fail if RenderWare has already been started = loaded by MSS
	if(gtaversion != SA_10 && Camera){
		MessageBox(NULL, "The portability plugin cannot be loaded by the default Mss32 ASI loader.\nUse another loader.", "Error", MB_ICONERROR | MB_OK);
		return FALSE;
	}

	if(gtaversion == III_10){
		// also patched by SilentPatch. so better replace the calls instead
		//InjectHook(0x580BB0, getUserFilesDir, PATCH_JUMP);
		InjectHook(0x479080, getUserFilesDir);
		InjectHook(0x5811DD, getUserFilesDir);
		InjectHook(0x591F10, getUserFilesDir);
	}else if(gtaversion == VC_10){
//		InjectHook(0x602240, getUserFilesDir, PATCH_JUMP);

		InjectHook(0x48E020, getUserFilesDir);
		InjectHook(0x61D8CA, getUserFilesDir);

		InjectHook(0x601A40, 0x601B2A, PATCH_JUMP);
		InterceptCall(&getSaveFileName_orig, getSaveFileName, 0x601B30);
	}else if(gtaversion == SA_10){
		InjectHook(0x744FB0, getUserFilesDir, PATCH_JUMP);
	}

	return RsEventHandler_orig(a, b);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		if(AddressByVersion<int>(1, 0, 0, 1, 0, 0, 1))
			InterceptCall(&RsEventHandler_orig, delayedPatches, AddressByVersion<int>(0x58275E, 0, 0, 0x5FFAFE, 0, 0, 0x748739));
	}
	return TRUE;
}
