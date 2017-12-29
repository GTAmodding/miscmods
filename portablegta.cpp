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

void
patch(void)
{
	char *p;
	GetModuleFileName(NULL, userPath, MAX_PATH);
	p = strrchr(userPath, '\\');
	strcpy(p+1, "userfiles");

	// Fail if RenderWare has already been started = loaded by MSS
	if(gtaversion != SA_10 && Camera){
		MessageBox(NULL, "The portability plugin cannot be loaded by the default Mss32 ASI loader.\nUse another loader.", "Error", MB_ICONERROR | MB_OK);
		return;
	}

	if(gtaversion == III_10){
		InjectHook(0x580BB0, getUserFilesDir, PATCH_JUMP);
	}else if(gtaversion == VC_10){
		InjectHook(0x602240, getUserFilesDir, PATCH_JUMP);
		InjectHook(0x601A40, 0x601B2A, PATCH_JUMP);
		InterceptCall(&getSaveFileName_orig, getSaveFileName, 0x601B30);
	}else if(gtaversion == SA_10){
		InjectHook(0x744FB0, getUserFilesDir, PATCH_JUMP);
	}
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		AddressByVersion<int>(0, 0, 0, 0, 0, 0, 0);
		patch();
	}
	return TRUE;
}
