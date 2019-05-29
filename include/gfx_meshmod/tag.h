#pragma once
#ifndef GFX_MESHMOD_TAG_H
#define GFX_MESHMOD_TAG_H

#include "al2o3_platform/platform.h"

typedef uint64_t MeshMod_Tag;

#define MESHMOD_TAG(a, b, c, d, e, f, g, version) (MeshMod_Tag)( \
		(uint64_t)(a) << 56 | (uint64_t)(b) << 48 | (uint64_t)(c) << 40 | (uint64_t)(d) << 32 | \
		(uint64_t)(e) << 24 | (uint64_t)(f) << 16 | (uint64_t)(g) << 8 | (uint64_t)(version) << 0 )

typedef void* MeshMod_TagRegistryHandle;
AL2O3_EXTERN_C MeshMod_TagRegistryHandle MeshMod_TagRegistryCreate();
AL2O3_EXTERN_C void MeshMod_TagRegistryDestroy(MeshMod_TagRegistryHandle handle);
AL2O3_EXTERN_C void MeshMod_TagRegistryAddTag(
		MeshMod_TagRegistryHandle handle,
		MeshMod_Tag tag,
		size_t dataSize,
		char const* description);

AL2O3_EXTERN_C bool MeshMod_TagRegistryTagExists(MeshMod_TagRegistryHandle handle, MeshMod_Tag tag);

AL2O3_EXTERN_C size_t MeshMod_TagRegistryDataSize(MeshMod_TagRegistryHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C char const* MeshMod_TagRegistryDescription(MeshMod_TagRegistryHandle handle, MeshMod_Tag tag);
#endif