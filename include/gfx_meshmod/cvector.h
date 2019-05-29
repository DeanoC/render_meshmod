#pragma once
#ifndef GFX_MESHMOD_CVECTOR_H
#define GFX_MESHMOD_CVECTOR_H

#include "al2o3_platform/platform.h"
#include "gfx_meshmod/meshmod.h"

typedef void* MeshMod_CVectorHandle;

AL2O3_EXTERN_C MeshMod_CVectorHandle MeshMod_CVectorCreate(size_t elementSize);
AL2O3_EXTERN_C void MeshMod_CVectorDestroy(MeshMod_CVectorHandle handle);

AL2O3_EXTERN_C size_t MeshMod_CVectorElementSizeFrom(MeshMod_CVectorHandle handle);

AL2O3_EXTERN_C size_t MeshMod_CVectorSizeFrom(MeshMod_CVectorHandle handle);
AL2O3_EXTERN_C void MeshMod_CVectorResize(MeshMod_CVectorHandle handle, size_t size);
AL2O3_EXTERN_C bool MeshMod_CVectorIsEmpty(MeshMod_CVectorHandle handle);

AL2O3_EXTERN_C size_t MeshMod_CVectorCapacityFrom(MeshMod_CVectorHandle handle);
AL2O3_EXTERN_C void MeshMod_CVectorReserve(MeshMod_CVectorHandle handle, size_t size);

AL2O3_EXTERN_C void* MeshMod_CVectorElementFrom(MeshMod_CVectorHandle handle, size_t index);

AL2O3_EXTERN_C size_t MeshMod_CVectorPushElement(MeshMod_CVectorHandle handle, void* element);
AL2O3_EXTERN_C size_t MeshMod_CVectorPushElementClone(MeshMod_CVectorHandle handle, size_t srcIndex);
AL2O3_EXTERN_C void MeshMod_CVectorReplace(MeshMod_CVectorHandle handle, size_t srcIndex,	size_t dstIndex);

AL2O3_EXTERN_C void MeshMod_CVectorPopElementFrom(MeshMod_CVectorHandle handle, void* out);
AL2O3_EXTERN_C void MeshMod_CVectorPopAndDiscardElementFrom(MeshMod_CVectorHandle handle);
AL2O3_EXTERN_C void* MeshMod_CVectorPeekElementFrom(MeshMod_CVectorHandle handle);

AL2O3_EXTERN_C void* MeshMod_CVectorDataFrom(MeshMod_CVectorHandle handle);

#endif // GFX_MESHMOD_MESH_H