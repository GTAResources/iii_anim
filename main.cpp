#include "iii_anim.h"

HMODULE dllModule, hDummyHandle;

void **&RwEngineInst = *(void***)0x661228;

WRAPPER void *RwMallocAlign(uint, int) { EAXJMP(0x526FD0); }
WRAPPER void RwFreeAlign(void*) { EAXJMP(0x527000); }
WRAPPER void gtadelete(void*) { EAXJMP(0x5A07E0); }
WRAPPER void *gta_nw(int) { EAXJMP(0x5A0690); }

WRAPPER RwBool RpClumpDestroy(RpClump*) { EAXJMP(0x59F500); }

WRAPPER void *GetModelFromName(char *name) { EAXJMP(0x4010D0); }
WRAPPER int CFileMgr::OpenFile(const char*, const char*) { EAXJMP(0x479100); }
WRAPPER int CFileMgr::Read(int, void*, int) { EAXJMP(0x479140); }
WRAPPER void CFileMgr::Seek(int, int, int) { EAXJMP(0x479180); }
WRAPPER void CFileMgr::CloseFile(int) { EAXJMP(0x479200); }
WRAPPER void CQuaternion::Slerp(CQuaternion&, CQuaternion&, float, float, float) { EAXJMP(0x4BA1C0); }
//WRAPPER void CQuaternion::Get(RwMatrix *mat) { EAXJMP(0x4DFD30); }

void **CModelInfo__ms_modelInfoPtrs = (void**)0x83D408;

static char *charclasses = (char*)0x618370;
static char *charset = (char*)0x618470;

int
gtastrcmp(const char *s1, const char *s2)
{
	char c1, c2;
	while(*s1){
		c1 = charclasses[*s1] & 0x40 ? *s1++ - 0x20 : *s1++;
		c2 = charclasses[*s2] & 0x40 ? *s2++ - 0x20 : *s2++;
		if(c1 != c2)
			return 1;
	}
	return 0;
}

int
lcstrcmp(const char *s1, const char *s2)
{
	int c1, c2;
	do{
		c1 = *s1 == -1 ? -1 : charset[*s1];
		c2 = *s2 == -1 ? -1 : charset[*s2];
		if(c1 < c2)
			return -1;
		if(c1 > c2)
			return 1;
		s1++;
		s2++;
	}while(c1);
	return 0;
}

void
patch10(void)
{
	if(sizeof(CAnimBlendSequence) != 0x2C ||
	   sizeof(CAnimBlendHierarchy) != 0x28 ||
	   sizeof(CAnimBlock) != 0x20 ||
	   sizeof(CAnimBlendNode) != 0x1C ||
	   sizeof(CAnimBlendAssociation) != 0x40 ||
	   sizeof(CAnimBlendAssocGroup) != 0x8 ||
	   sizeof(AnimBlendFrameData) != 0x14 ||
	   sizeof(CAnimBlendClumpData) != 0x14 ||
	   sizeof(RFrame) != 0x14 ||
	   sizeof(RTFrame) != 0x20){
		printf("SIZE MISMATCH\n");
		return;
	}
	printf("sizes ok\n");

	MemoryVP::InjectHook(0x401000, &CAnimBlendAssocGroup::ctor, PATCH_JUMP);
	MemoryVP::InjectHook(0x401130, static_cast<void(CAnimBlendAssocGroup::*)(const char*)>(&CAnimBlendAssocGroup::CreateAssociations), PATCH_JUMP);
	MemoryVP::InjectHook(0x401220, static_cast<void(CAnimBlendAssocGroup::*)(const char *name, RpClump *clump, char **names, int numAnims)>(&CAnimBlendAssocGroup::CreateAssociations), PATCH_JUMP);
	MemoryVP::InjectHook(0x4012D0, &CAnimBlendAssocGroup::dtor, PATCH_JUMP);
	MemoryVP::InjectHook(0x401300, static_cast<CAnimBlendAssociation*(CAnimBlendAssocGroup::*)(const char*)>(&CAnimBlendAssocGroup::GetAnimation), PATCH_JUMP);
	MemoryVP::InjectHook(0x4013D0, static_cast<CAnimBlendAssociation*(CAnimBlendAssocGroup::*)(uint)>(&CAnimBlendAssocGroup::GetAnimation), PATCH_JUMP);
	MemoryVP::InjectHook(0x4013E0, static_cast<CAnimBlendAssociation*(CAnimBlendAssocGroup::*)(const char*)>(&CAnimBlendAssocGroup::CopyAnimation), PATCH_JUMP);
	MemoryVP::InjectHook(0x401420, static_cast<CAnimBlendAssociation*(CAnimBlendAssocGroup::*)(uint)>(&CAnimBlendAssocGroup::CopyAnimation), PATCH_JUMP);

	MemoryVP::InjectHook(0x401460, &CAnimBlendAssociation::ctor, PATCH_JUMP);
	MemoryVP::InjectHook(0x401520, &CAnimBlendAssociation::dtor, PATCH_JUMP);
	MemoryVP::InjectHook(0x401560, static_cast<void(CAnimBlendAssociation::*)(RpClump *clump, CAnimBlendHierarchy *anim)>(&CAnimBlendAssociation::Init), PATCH_JUMP);
	MemoryVP::InjectHook(0x401620, static_cast<void(CAnimBlendAssociation::*)(CAnimBlendAssociation&)>(&CAnimBlendAssociation::Init), PATCH_JUMP);
	MemoryVP::InjectHook(0x4016A0, &CAnimBlendAssociation::AllocateAnimBlendNodeArray, PATCH_JUMP);
	MemoryVP::InjectHook(0x4016F0, &CAnimBlendAssociation::FreeAnimBlendNodeArray, PATCH_JUMP);
	MemoryVP::InjectHook(0x401700, &CAnimBlendAssociation::SetCurrentTime, PATCH_JUMP);
	MemoryVP::InjectHook(0x401780, &CAnimBlendAssociation::SyncAnimation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4017B0, &CAnimBlendAssociation::GetNode, PATCH_JUMP);
	MemoryVP::InjectHook(0x4017D0, &CAnimBlendAssociation::Start, PATCH_JUMP);
	MemoryVP::InjectHook(0x4017E0, &CAnimBlendAssociation::SetBlend, PATCH_JUMP);
	MemoryVP::InjectHook(0x401800, &CAnimBlendAssociation::SetDeleteCallback, PATCH_JUMP);
	MemoryVP::InjectHook(0x401820, &CAnimBlendAssociation::SetFinishCallback, PATCH_JUMP);
	MemoryVP::InjectHook(0x401840, &CAnimBlendAssociation::dtor2, PATCH_JUMP);
	MemoryVP::InjectHook(0x4031F0, &CAnimBlendAssociation::UpdateTime, PATCH_JUMP);
	MemoryVP::InjectHook(0x4032B0, &CAnimBlendAssociation::UpdateBlend, PATCH_JUMP);

	MemoryVP::InjectHook(0x401880, &CAnimBlendClumpData::ctor, PATCH_JUMP);
	MemoryVP::InjectHook(0x4018B0, &CAnimBlendClumpData::dtor, PATCH_JUMP);
	MemoryVP::InjectHook(0x4018F0, &CAnimBlendClumpData::SetNumberOfFrames, PATCH_JUMP);
	MemoryVP::InjectHook(0x401930, &CAnimBlendClumpData::ForAllFrames, PATCH_JUMP);

	MemoryVP::InjectHook(0x4019A0, &CAnimBlendHierarchy::Shutdown, PATCH_JUMP);
	MemoryVP::InjectHook(0x4019C0, &CAnimBlendHierarchy::SetName, PATCH_JUMP);
	MemoryVP::InjectHook(0x401AB0, &CAnimBlendHierarchy::RemoveAnimSequences, PATCH_JUMP);
	MemoryVP::InjectHook(0x4019E0, &CAnimBlendHierarchy::CalcTotalTime, PATCH_JUMP);
	MemoryVP::InjectHook(0x401A80, &CAnimBlendHierarchy::RemoveQuaternionFlips, PATCH_JUMP);
	MemoryVP::InjectHook(0x401AD0, &CAnimBlendHierarchy::Uncompress, PATCH_JUMP);
	MemoryVP::InjectHook(0x401B00, &CAnimBlendHierarchy::RemoveUncompressedData, PATCH_JUMP);

	MemoryVP::InjectHook(0x401B10, &CAnimBlendNode::Init, PATCH_JUMP);
	MemoryVP::InjectHook(0x401B30, &CAnimBlendNode::Update, PATCH_JUMP);
	MemoryVP::InjectHook(0x401DC0, &CAnimBlendNode::NextKeyFrame, PATCH_JUMP);
	MemoryVP::InjectHook(0x401E70, &CAnimBlendNode::CalcDeltas, PATCH_JUMP);
	MemoryVP::InjectHook(0x401FE0, &CAnimBlendNode::GetCurrentTranslation, PATCH_JUMP);
	MemoryVP::InjectHook(0x402110, &CAnimBlendNode::GetEndTranslation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4021B0, &CAnimBlendNode::FindKeyFrame, PATCH_JUMP);

	MemoryVP::InjectHook(0x4022D0, &CAnimBlendSequence::ctor, PATCH_JUMP);
	MemoryVP::InjectHook(0x402300, &CAnimBlendSequence::dtor, PATCH_JUMP);
	MemoryVP::InjectHook(0x402330, &CAnimBlendSequence::SetName, PATCH_JUMP);
	MemoryVP::InjectHook(0x402350, &CAnimBlendSequence::SetNumFrames, PATCH_JUMP);
	MemoryVP::InjectHook(0x4023A0, &CAnimBlendSequence::RemoveQuaternionFlips, PATCH_JUMP);
	MemoryVP::InjectHook(0x402470, &CAnimBlendSequence::dtor2, PATCH_JUMP);

	MemoryVP::InjectHook(0x403380, CAnimManager::Initialise, PATCH_JUMP);
	MemoryVP::InjectHook(0x4033B0, CAnimManager::Shutdown, PATCH_JUMP);
	MemoryVP::InjectHook(0x403410, CAnimManager::UncompressAnimation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4034A0, CAnimManager::GetAnimationBlock, PATCH_JUMP);
	MemoryVP::InjectHook(0x4034F0, CAnimManager::GetAnimation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4035B0, CAnimManager::GetAnimGroupName, PATCH_JUMP);
	MemoryVP::InjectHook(0x4035C0, CAnimManager::CreateAnimAssociation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4035E0, (CAnimBlendAssociation *(*)(int, int))CAnimManager::GetAnimAssociation, PATCH_JUMP);
	MemoryVP::InjectHook(0x403600, (CAnimBlendAssociation *(*)(int, const char*))CAnimManager::GetAnimAssociation, PATCH_JUMP);
	MemoryVP::InjectHook(0x403620, CAnimManager::AddAnimation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4036A0, CAnimManager::AddAnimationAndSync, PATCH_JUMP);
	MemoryVP::InjectHook(0x403710, CAnimManager::BlendAnimation, PATCH_JUMP);
	MemoryVP::InjectHook(0x4038F0, CAnimManager::LoadAnimFiles, PATCH_JUMP);
	MemoryVP::InjectHook(0x403A10, (void (*)(const char*))CAnimManager::LoadAnimFile, PATCH_JUMP);
	MemoryVP::InjectHook(0x403A40, (void (*)(int, bool))CAnimManager::LoadAnimFile, PATCH_JUMP);
	MemoryVP::InjectHook(0x404320, CAnimManager::RemoveLastAnimFile, PATCH_JUMP);

	MemoryVP::InjectHook(0x405430, RpAnimBlendClumpFindFrame, PATCH_JUMP);
}

BOOL WINAPI
DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH){
		dllModule = hInst;

/*		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);*/

		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)&DllMain, &hDummyHandle);

		if (*(DWORD*)0x5C1E75 == 0xB85548EC)	// 1.0
			patch10();
		else
			return FALSE;
	}

	return TRUE;
}