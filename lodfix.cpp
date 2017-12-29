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

struct CBaseModelInfo
{
	void **vtable;
	char     name[24];
	void *colModel;
	void *twodeffect;
	short id;
	ushort refCount;
	short txdSlot;
	uchar type;
	uchar num2dEffects;
	bool freeCol;
};
static_assert(sizeof(CBaseModelInfo) == 0x30, "CBaseModelInfo: wrong size");

struct CSimpleModelInfo : public CBaseModelInfo
{
	void *m_atomics[3];
	float m_lodDistances[3];
	uchar m_numAtomics;
	uchar m_alpha;
	ushort m_flags;

	void FindRelatedModel(void);
	float GetLargestLodDistance(void);
	void SetupBigBuilding(void);
};
static_assert(sizeof(CSimpleModelInfo) == 0x4C, "CSimpleModelInfo: wrong size");

void *TheCamera = (void*)0x6FACF8;

WRAPPER void CSimpleModelInfo::FindRelatedModel(void) { EAXJMP(0x517C00); }
WRAPPER float CSimpleModelInfo::GetLargestLodDistance(void) { EAXJMP(0x517A60); }

void
CSimpleModelInfo::SetupBigBuilding(void)
{
	CSimpleModelInfo *related;
	if(m_lodDistances[0] > 300.0f && m_atomics[2] == nil){
		m_flags |= 0x10;	// isBigBuilding
		FindRelatedModel();
		related = (CSimpleModelInfo*)this->m_atomics[2];
		if(related)
			m_lodDistances[2] = related->GetLargestLodDistance()/FIELD(float, TheCamera, 0xEC);	// lodMult
		else{
			if(strstr(this->name, "LOD"))
				m_lodDistances[2] = 100.0f;
			else
				m_lodDistances[2] = 0.0f;
		}
	}
}

void
patchIII10(void)
{
	InjectHook(0x476D96,& CSimpleModelInfo::SetupBigBuilding);
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
