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

#define RADTODEG(x) (float)(180.0f*(x)/M_PI)
#define DEGTORAD(x) (float)((x)/180.0f*M_PI)


int gtaversion = -1;

struct GlobalScene
{
	void *world;
	void *camera;
};
GlobalScene &Scene = *AddressByVersion<GlobalScene*>(0x726768, 0, 0, 0x8100B8, 0, 0, 0xC17038);

float FOV = 70.0f;


void
patchAllFOV(void)
{
	// cam on a string
	Patch<float>(0x45C11A + 6, FOV);
	// follow ped
	Patch<float>(0x45E531 + 6, FOV);
	// follow ped with mouse
	Patch<float>(0x45FF88 + 6, FOV);
	// fight cam
	Patch<float>(0x45D312 + 6, FOV);
	// debug (although overriden...)
	Patch<float>(0x45CCF9 + 6, FOV);
	// syphon crim in front
	Patch<float>(0x463A7B + 6, FOV);
	// special fixed for syphon
	Patch<float>(0x463119 + 6, FOV);
	// syphon
	Patch<float>(0x46313F + 6, FOV);
	// behind car (where is this used?)
	Patch<float>(0x45BE6D + 6, FOV);
	// behind boat
	Patch<float>(0x45B52E + 6, FOV);
	// fixed
	Patch<float>(0x45DB6E + 6, FOV);

	// WS fix FOV control
//	Patch<float>(0x513547, FOV);
}

float hFov2vFov(float hfov)
{
	void *raster = FIELD(void*, Scene.camera, 0x60);
	float w = FIELD(float, raster, 0xC);
	float h = FIELD(float, raster, 0x10);

	// => tan(hFOV/2) = tan(vFOV/2)*aspectRatio
	// => tan(vFOV/2) = tan(hFOV/2)/aspectRatio
	float ar1 = 4.0/3.0;
	float ar2 = w/h;
	hfov = DEGTORAD(hfov);
	float vfov = atan(tan(hfov/2) / ar1) *2;
	hfov = atan(tan(vfov/2) * ar2) *2;
	return RADTODEG(hfov);
}
float &CDraw__ms_fFOV = *(float*)AddressByVersion<float*>(0x5FBC6C, 0, 0, 0x696658, 0, 0, 0x8D5038);
void (*CDraw__SetFOV_hook)(float fov);
void CDraw__SetFOV(float fov)
{
	CDraw__ms_fFOV = hFov2vFov(fov);
}
void CDraw__SetFOV_chain(float fov)
{
	CDraw__SetFOV_hook(hFov2vFov(fov));
}

char dllName[MAX_PATH];
bool wasHooked;

void
patchIII10(void)
{
	char *s, *t;
	s = strrchr(dllName, '_');
	if(s == NULL) goto set;
	*s++ = '\0';
	t = strchr(s, '.');
	if(t == NULL) goto set;
	*t = '\0';
	FOV = atoi(s);

set:
	uint32 addr = 0x4FE7B0;
	// convert FOV to aspect ratio
	if(*(uint8*)addr == 0xE9){
		// Sombody already overwrote SetFOV with a jump, we assume it was WS fix
// this does not seem to work very well...peds don't spawn correctly, WHY?
//		ExtractCall(&CDraw__SetFOV_hook, addr);
//		InjectHook(addr, CDraw__SetFOV_chain, PATCH_JUMP);
	}else{
		// ignore aspect ratio in CameraSize and use raster dimensions instead
		// otherwise hFov2vFov won't give expected results
		InjectHook(0x5272BA, 0x5272D2, PATCH_JUMP);

		InjectHook(addr, CDraw__SetFOV, PATCH_JUMP);
	}

	patchAllFOV();
}

void
patchVC10(void)
{
	uint32 addr = 0x54A2E0;
	// convert FOV to aspect ratio
	if(*(uint8*)addr == 0xE9){
		// Sombody already overwrote SetFOV with a jump, we assume it was WS fix
	}else{
		// ignore aspect ratio in CameraSize and use raster dimensions instead
		// otherwise hFov2vFov won't give expected results
		InjectHook(0x580900, 0x580918, PATCH_JUMP);

		InjectHook(addr, CDraw__SetFOV, PATCH_JUMP);
	}
}

void
patchSA10(void)
{
	uint32 addr = 0x6FF410;
	// convert FOV to aspect ratio
	if(*(uint8*)addr == 0xE9){
		// Sombody already overwrote SetFOV with a jump, we assume it was WS fix
	}else{
		// ignore aspect ratio in CameraSize and use raster dimensions instead
		// otherwise hFov2vFov won't give expected results
		InjectHook(0x72FCFE, 0x72FD16, PATCH_JUMP);

		InjectHook(addr, CDraw__SetFOV, PATCH_JUMP);
	}
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		AddressByVersion<uint32_t>(0, 0, 0, 0, 0, 0, 0);
		GetModuleFileName(hInst, dllName, MAX_PATH);
		switch(gtaversion){
		case III_10:
			patchIII10();
			break;
		case VC_10:
			patchVC10();
			break;
		case SA_10:
			patchSA10();
			break;
		}
	}

	return TRUE;
}
