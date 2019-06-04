#pragma once
#ifndef GFX_MESHMOD_TAG_H
#define GFX_MESHMOD_TAG_H

#include "al2o3_platform/platform.h"
#include "gfx_meshmod/meshmod.h"

#define MESHMOD_TAG(a, b, c, d, e, f) (MeshMod_Tag)( \
		(uint64_t)(a) << 56 | (uint64_t)(b) << 48 | (uint64_t)(c) << 40 | (uint64_t)(d) << 32 | (uint64_t)(e) << 24 | (uint64_t)(f) << 16)
#define MESHMOD_VERTEXTAG(b, c, d, e, f) (MeshMod_Tag)( \
		(uint64_t)('V') << 56 | (uint64_t)(b) << 48 | (uint64_t)(c) << 40 | (uint64_t)(d) << 32 | (uint64_t)(e) << 24 | (uint64_t)(f) << 16)
#define MESHMOD_EDGETAG(b, c, d, e, f) (MeshMod_Tag)( \
		(uint64_t)('E') << 56 | (uint64_t)(b) << 48 | (uint64_t)(c) << 40 | (uint64_t)(d) << 32 | (uint64_t)(e) << 24 | (uint64_t)(f) << 16)
#define MESHMOD_POLYGONTAG(b, c, d, e, f) (MeshMod_Tag)( \
		(uint64_t)('P') << 56 | (uint64_t)(b) << 48 | (uint64_t)(c) << 40 | (uint64_t)(d) << 32 | (uint64_t)(e) << 24 | (uint64_t)(f) << 16)
#undef MESHMOD_TAG
	
typedef struct MeshMod_Registry* MeshMod_RegistryHandle;

typedef void const *(*MeshMod_RegistryDefaultDataFunc)();
typedef char const *(*MeshMod_RegistryDescriptionFunc)();
typedef bool (*MeshMod_RegistryEqualFunc)(void const* a, void const* b, float const epsilon);
typedef void * (*MeshMod_RegistryDestroyFunc)(void* element);

typedef void (*MeshMod_RegistryVertexInterpolate1DFunc)(void const* a, void const* b, void* r, float const t);
typedef void (*MeshMod_RegistryVertexInterpolate2DFunc)(void const* a, void const* b, void const* c, void* r, float const u, float const v);

typedef struct MeshMod_RegistryCommonFunctionTable {
	MeshMod_RegistryDefaultDataFunc defaultDataFunc;
	MeshMod_RegistryEqualFunc equalFunc;
	MeshMod_RegistryDestroyFunc destroyFunc; // can be null
	MeshMod_RegistryDescriptionFunc descriptionFunc; // can be null
} MeshMod_RegistryCommonFunctionTable;

typedef struct MeshMod_RegistryVertexFunctionTable {
	MeshMod_RegistryVertexInterpolate1DFunc interpolate1DFunc;
	MeshMod_RegistryVertexInterpolate2DFunc interpolate2DFunc;
} MeshMod_RegistryVertexFunctionTable;

typedef struct MeshMod_RegistryEdgeFunctionTable {
	int placeholder; // because C requires 1 member
} MeshMod_RegistryEdgeFunctionTable;

typedef struct MeshMod_RegistryPolygonFunctionTable {
	int placeholder; // because C requires 1 member
} MeshMod_RegistryPolygonFunctionTable;

AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_RegistryCreate();
AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_RegistryCreateWithDefaults();

AL2O3_EXTERN_C void MeshMod_RegistryDestroy(MeshMod_RegistryHandle handle);

AL2O3_EXTERN_C void MeshMod_RegistryAddType(
	MeshMod_RegistryHandle handle,
	MeshMod_Tag tag,
	size_t elementSize,
	MeshMod_RegistryCommonFunctionTable* commonFunctionTable,
	void* functionTable);


AL2O3_EXTERN_C bool MeshMod_RegistryExists(MeshMod_RegistryHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C size_t MeshMod_RegistryElementSize(MeshMod_RegistryHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C MeshMod_Type MeshMod_RegistryType(MeshMod_RegistryHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C char const* MeshMod_RegistryDescription(MeshMod_RegistryHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C void const* MeshMod_RegistryDefaultData(MeshMod_RegistryHandle handle, MeshMod_Tag tag);
AL2O3_EXTERN_C MeshMod_RegistryCommonFunctionTable* MeshMod_RegistryGetCommonFunctionTable(MeshMod_RegistryHandle handle, MeshMod_Tag tag);

AL2O3_EXTERN_C void* MeshMod_RegistryFunctionTable(MeshMod_RegistryHandle handle, MeshMod_Tag tag, MeshMod_Type type);

#endif