#pragma once
#ifndef GFX_MESHMOD_CDICT_H
#define GFX_MESHMOD_CDICT_H

#include "al2o3_platform/platform.h"
#include "gfx_meshmod/meshmod.h"

#define MESHMOD_DECLARE_DICT(postfix, keytype, valuetype) \
typedef struct MeshMod_CDict##postfix *MeshMod_CDict##postfix##Handle; \
AL2O3_EXTERN_C MeshMod_CDict##postfix##Handle MeshMod_CDict##postfix##Create(); \
AL2O3_EXTERN_C void MeshMod_CDict##postfix##Destroy(MeshMod_CDict##postfix##Handle handle); \
AL2O3_EXTERN_C size_t MeshMod_CDict##postfix##Size(MeshMod_CDict##postfix##Handle handle); \
AL2O3_EXTERN_C valuetype MeshMod_CDict##postfix##GetByIndex(MeshMod_CDict##postfix##Handle handle, size_t index); \
AL2O3_EXTERN_C bool MeshMod_CDict##postfix##IsEmpty(MeshMod_CDict##postfix##Handle handle); \
AL2O3_EXTERN_C size_t MeshMod_CDict##postfix##Capacity(MeshMod_CDict##postfix##Handle handle); \
AL2O3_EXTERN_C void MeshMod_CDict##postfix##Reserve(MeshMod_CDict##postfix##Handle handle, size_t const size); \
AL2O3_EXTERN_C bool MeshMod_CDict##postfix##KeyExists(MeshMod_CDict##postfix##Handle handle, keytype const key); \
AL2O3_EXTERN_C valuetype MeshMod_CDict##postfix##Get(MeshMod_CDict##postfix##Handle handle, keytype const key); \
AL2O3_EXTERN_C bool MeshMod_CDict##postfix##Lookup(MeshMod_CDict##postfix##Handle handle, keytype const key, valuetype* out); \
AL2O3_EXTERN_C bool MeshMod_CDict##postfix##Add(MeshMod_CDict##postfix##Handle handle, keytype const key, valuetype const in); \
AL2O3_EXTERN_C void MeshMod_CDict##postfix##Remove(MeshMod_CDict##postfix##Handle handle, keytype const key);

MESHMOD_DECLARE_DICT(U32, uint32_t, uint32_t)
MESHMOD_DECLARE_DICT(U64, uint64_t, uint64_t)

#undef MESHMOD_DECLARE_DICT

#endif // GFX_MESHMOD_MESH_H