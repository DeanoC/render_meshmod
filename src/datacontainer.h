#pragma once

#include "al2o3_cadt/bagofvectors.h"
#include "al2o3_cadt/dict.h"

typedef struct MeshMod_DataContainer {
	MeshMod_Type containerType;

	CADT_BagOfVectorsHandle bag;
	CADT_DictU64Handle vectorHashs;
	CADT_DictU64Handle userData; // 64 bit of user data per vector

	MeshMod_MeshHandle owner;
	Handle_Manager64* handleManager;
} MeshMod_DataContainer;

typedef struct { struct MeshMod_DataContainer* dc; } MeshMod_VertexDataContainerHandle;
typedef struct { struct MeshMod_DataContainer* dc; } MeshMod_EdgeDataContainerHandle;
typedef struct { struct MeshMod_DataContainer* dc; } MeshMod_PolygonDataContainerHandle;

// we reserve 4 bits of the user data for transitive etc. so user data is 60 bit per tag
#define MeshMod_DataContainerUserDataMask 0xF00000000000000ull
#define MeshMod_DataContainerTransitiveFlag 0x800000000000000ull

AL2O3_EXTERN_C MeshMod_DataContainer* MeshMod_DataContainerCreate(MeshMod_MeshHandle handle, MeshMod_Type type);
AL2O3_EXTERN_C void MeshMod_DataContainerDestroy(MeshMod_DataContainer* dc);
AL2O3_EXTERN_C MeshMod_DataContainer* MeshMod_DataContainerClone(MeshMod_DataContainer* dc, MeshMod_MeshHandle owner);

AL2O3_EXTERN_C void MeshMod_DataContainerMarkChanged(MeshMod_DataContainer* dc);

AL2O3_EXTERN_C Handle_Handle64 MeshMod_DataContainerAlloc(MeshMod_DataContainer* dc);
AL2O3_EXTERN_C void MeshMod_DataContainerRelease(MeshMod_DataContainer* dc, Handle_Handle64 handle);
AL2O3_EXTERN_C bool MeshMod_DataContainerIsValid(MeshMod_DataContainer* dc, Handle_Handle64 handle);
AL2O3_EXTERN_C void MeshMod_DataContainerReplace(MeshMod_DataContainer* dc, Handle_Handle64 srcHandle, Handle_Handle64 dstHandle);
AL2O3_EXTERN_C void MeshMod_DataContainerSwap(MeshMod_DataContainer* dc, Handle_Handle64 handle0, Handle_Handle64 handle1);

AL2O3_EXTERN_C void MeshMod_DataContainerTagEnsure(MeshMod_DataContainer* dc, MeshMod_Tag tag);
AL2O3_EXTERN_C bool MeshMod_DataContainerTagExists(MeshMod_DataContainer* dc, MeshMod_Tag tag);
AL2O3_EXTERN_C void MeshMod_DataContainerTagRemove(MeshMod_DataContainer* dc, MeshMod_Tag tag);
AL2O3_EXTERN_C uint64_t MeshMod_DataContainerTagGetUserData(MeshMod_DataContainer* dc, MeshMod_Tag tag);
AL2O3_EXTERN_C uint64_t MeshMod_DataContainerTagGetOrComputeHash(MeshMod_DataContainer* dc, MeshMod_Tag tag);
AL2O3_EXTERN_C CADT_VectorHandle MeshMod_DataContainerTagToPtr(MeshMod_DataContainer* dc, MeshMod_Tag tag);
AL2O3_EXTERN_C void MeshMod_DataContainerTagSetUserData(MeshMod_DataContainer* dc, MeshMod_Tag tag, uint64_t userData);
AL2O3_EXTERN_C void MeshMod_DataContainerTagSetTransitive(MeshMod_DataContainer* dc, MeshMod_Tag tag, bool transitive);
AL2O3_EXTERN_C void MeshMod_DataContainerTagHandleToDefault(MeshMod_DataContainer* dc, MeshMod_Tag tag, Handle_Handle64 handle);
AL2O3_EXTERN_C void* MeshMod_DataContainerTagHandleToPtr(MeshMod_DataContainer* dc, MeshMod_Tag tag, Handle_Handle64 handle);

// how many indices (alloc'ed or not)
AL2O3_EXTERN_C uint64_t MeshMod_DataContainerIndexCount(MeshMod_DataContainer* dc);
AL2O3_EXTERN_C Handle_Handle64 MeshMod_DataContainerIndexToHandle(MeshMod_DataContainer* dc, uint64_t actualIndex);

AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_VertexDataContainerInterpolate1D(MeshMod_VertexDataContainerHandle handle, MeshMod_VertexHandle i0, MeshMod_VertexHandle i1, float t);
AL2O3_EXTERN_C MeshMod_VertexHandle MeshMod_VertexDataContainerInterpolate2D(MeshMod_VertexDataContainerHandle handle, MeshMod_VertexHandle i0, MeshMod_VertexHandle i1, MeshMod_VertexHandle i2, float u, float v);

