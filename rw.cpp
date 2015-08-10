#include "iii_anim.h"

RwFreeList *&RpHAnimAnimationFreeList = *(RwFreeList**)0x9405B0;
RpHAnimInterpolatorInfo *RpHAnimInterpolatorInfoBlock = (RpHAnimInterpolatorInfo*)0x661608;
RwInt32 &RpHAnimInterpolatorInfoBlockNumEntries = *(RwInt32*)0x661888;

RpHAnimInterpolatorInfo*
RpHAnimGetInterpolatorInfo(RwInt32 typeID)
{
	for(RwInt32 i = 0; i < RpHAnimInterpolatorInfoBlockNumEntries; i++)
		if(RpHAnimInterpolatorInfoBlock[i].typeID == typeID)
			return &RpHAnimInterpolatorInfoBlock[i];
	return NULL;
}

RpHAnimAnimation*
RpHAnimAnimationCreate(RwInt32 typeID, RwInt32 numFrames, RwInt32 flags, RwReal duration)
{
	RpHAnimInterpolatorInfo *info = RpHAnimGetInterpolatorInfo(typeID);
	if(info == NULL)
		return NULL;
	RpHAnimAnimation *anim = (RpHAnimAnimation*)RwFreeListAlloc(RpHAnimAnimationFreeList);
	anim->duration = duration;
	anim->flags = flags;
	anim->interpInfo = info;
	anim->numFrames = numFrames;
	anim->pFrames = RwMalloc(numFrames*info->keyFrameSize);
	return anim;
}

RwInt32
RpHAnimIDGetIndex(RpHAnimHierarchy *hierarchy, RwInt32 ID)
{
	for(RwInt32 i = 0; i < hierarchy->numNodes; i++)
		if(hierarchy->pNodeInfo[i].nodeID == ID)
			return i;
	return -1;
}

const RwMatrixWeights*
RpSkinGetVertexBoneWeights(RpSkin *skin)
{
	return *(RwMatrixWeights**)((char*)skin + 12);
}

const RwMatrix*
RpSkinGetSkinToBoneMatrices(RpSkin *skin)
{
	return *(RwMatrix**)((char*)skin + 4);
}

RwUInt32
RpSkinGetNumBones(RpSkin *skin)
{
	return *(RwUInt32*)skin;
}
