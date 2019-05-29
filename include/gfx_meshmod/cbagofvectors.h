#pragma once
#ifndef GFX_MESHMOD_CBAGOFVECTORS_H
#define GFX_MESHMOD_CBAGOFVECTORS_H

#include "al2o3_platform/platform.h"
#include "gfx_meshmod/cvector.h"

typedef struct MeshMod_CBagOfVectors *MeshMod_CBagOfVectorsHandle;

AL2O3_EXTERN_C MeshMod_CBagOfVectorsHandle MeshMod_CBagOfVectorsCreate();
AL2O3_EXTERN_C void MeshMod_CBagOfVectorsDestroy(MeshMod_CBagOfVectorsHandle handle);

AL2O3_EXTERN_C MeshMod_CVectorHandle MeshMod_CBagOfVectorsAdd(MeshMod_CBagOfVectorsHandle handle, uint64_t key, size_t elementSize);
AL2O3_EXTERN_C void MeshMod_CBagOfVectorsOwnVector(MeshMod_CBagOfVectorsHandle handle, uint64_t key, MeshMod_CVectorHandle vector);
AL2O3_EXTERN_C void MeshMod_CBagOfVectorsRemove(MeshMod_CBagOfVectorsHandle handle, uint64_t key);
AL2O3_EXTERN_C MeshMod_CVectorHandle MeshMod_CBagOfVectorsGet(MeshMod_CBagOfVectorsHandle handle, uint64_t key);

AL2O3_EXTERN_C bool MeshMod_CBagOfVectorsKeyExists(MeshMod_CBagOfVectorsHandle handle, uint64_t key);
AL2O3_EXTERN_C size_t MeshMod_CBagOfVectorsKeyCount(MeshMod_CBagOfVectorsHandle handle);
AL2O3_EXTERN_C uint64_t MeshMod_CBagOfVectorsGetKey(MeshMod_CBagOfVectorsHandle handle, size_t index);


#endif