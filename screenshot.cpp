#pragma comment(lib, "user32")

#define _CRT_SECURE_NO_WARNINGS
#include <png.h>
#include <windows.h>
#include <rwcore.h>
#include <rwplcore.h>
#include <stdint.h>
#include <assert.h>
typedef uintptr_t uintptr;
#include "MemoryMgr_all.h"
#include <time.h>

#define nil NULL
typedef uint8_t uint8;

int gtaversion = -1;

static uint32_t RwImageCreate_A = AddressByVersion<uint32_t>(0x5A9120, 0, 0, 0x651250, 0, 0, 0x8026E0);
WRAPPER RwImage *RwImageCreate(RwInt32, RwInt32, RwInt32) { VARJMP(RwImageCreate_A); }
static uint32_t RwImageAllocatePixels_A = AddressByVersion<uint32_t>(0x5A91E0, 0, 0, 0x651310, 0, 0, 0x8027A0);
WRAPPER RwImage *RwImageAllocatePixels(RwImage *) { VARJMP(RwImageAllocatePixels_A); }
static uint32_t RwImageSetFromRaster_A = AddressByVersion<uint32_t>(0x5BBF10, 0, 0, 0x660270, 0, 0, 0x804250);
WRAPPER RwImage *RwImageSetFromRaster(RwImage*, RwRaster*) { VARJMP(RwImageSetFromRaster_A); }
static uint32_t RwImageDestroy_A = AddressByVersion<uint32_t>(0x5A9180, 0x5A9440, 0x5AB6A0, 0x6512B0, 0x651300, 0x650260, 0x802740);
WRAPPER RwBool RwImageDestroy(RwImage*) { VARJMP(RwImageDestroy_A); }

void
writePNG(RwImage *img, const char *filename)
{
	FILE *f;
	png_structp png;
	png_infop info;
	int x, y;
	png_byte **rows;
	uint8 *line;

	if(img->depth != 32)
		return;
	if(f = fopen(filename, "wb"), f == nil)
		return;

	png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nil, nil, nil);
	if(png == nil)
		goto fail1;

	info = png_create_info_struct(png);
	if(info == nil)
		goto fail2;

	if(setjmp(png_jmpbuf(png)))
		goto fail2;

	png_set_IHDR(png,
		info,
		img->width,
		img->height,
		8,
		PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	rows = (png_byte**)png_malloc(png, img->height * sizeof(png_byte*));
	line = img->cpPixels;
	for(y = 0; y < img->height; y++) {
		png_byte *row = (png_byte*)png_malloc(png, img->width * 3);
		rows[y] = row;
		for(x = 0; x < img->width; x++) {
			*row++ = line[x*4 + 0];
			*row++ = line[x*4 + 1];
			*row++ = line[x*4 + 2];
		}
		line += img->stride;
	}

	png_init_io(png, f);
	png_set_rows(png, info, rows);
	png_write_png(png, info, PNG_TRANSFORM_IDENTITY, nil);

	for(y = 0; y < img->height; y++)
		png_free(png, rows[y]);
	png_free(png, rows);

fail2:
	png_destroy_write_struct(&png, &info);
fail1:
	fclose(f);
}

RwImage*
psGrabScreen(RwCamera *cam)
{
	RwRaster *ras = cam->frameBuffer;
	assert(ras);
	RwImage *img = RwImageCreate(ras->width, ras->height, 32);
	assert(ras);
	RwImageAllocatePixels(img);
	RwImageSetFromRaster(img, ras);
	return img;
}

void
RwGrabScreen(RwCamera *cam)
{
	char name[256];
	RwImage *img = psGrabScreen(cam);
	sprintf(name, "snapshots/snap_%lld.png", time(nil));
	writePNG(img, name);
	RwImageDestroy(img);
}

void (*RsCameraShowRaster_orig)(RwCamera *cam);
void
RsCameraShowRasterHook(RwCamera *cam)
{
	{
		static bool keystate = false;
		if(GetAsyncKeyState(VK_SNAPSHOT) & 0x8000){
			if(!keystate){
				keystate = true;
				RwGrabScreen(cam);
			}
		}else
			keystate = false;
	}
	RsCameraShowRaster_orig(cam);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		if(AddressByVersion<int>(1, 0, 0, 1, 0, 0, 1))
			InterceptCall(&RsCameraShowRaster_orig, RsCameraShowRasterHook, AddressByVersion<int>(0x48D45C, 0, 0, 0x4A6168, 0, 0, 0x53EC01));
	}
	return TRUE;
}
