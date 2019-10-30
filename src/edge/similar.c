#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/edge/similar.h"
#include "render_meshmod/registry.h"

static void const* EdgeSimilarDefaultData() {
	static MeshMod_EdgeSimilar invalid = { 0 };
	return &invalid;
}

static bool EdgeSimilarEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_EdgeSimilar const* a = (MeshMod_EdgeSimilar const*)va;
	MeshMod_EdgeSimilar const* b = (MeshMod_EdgeSimilar const*)vb;
	return	Handle_HandleEqual64(a->handle, b->handle);
}

static char const* EdgeSimilarDescription() {
	return"Edge Similar";
}

AL2O3_EXTERN_C void MeshMod_EdgeSimilarAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
			.defaultDataFunc = &EdgeSimilarDefaultData,
			.equalFunc = &EdgeSimilarEqual,
			.destroyFunc = NULL,
			.descriptionFunc = &EdgeSimilarDescription,
			.distanceFunc = NULL
	};

	static MeshMod_RegistryEdgeFunctionTable EdgeFunctionTable = {
			0
	};

	MeshMod_RegistryAddType(handle,
													MeshMod_EdgeSimilarTag.tag,
			sizeof(MeshMod_EdgeSimilar),
			&CommonFunctionTable,
			&EdgeFunctionTable);
}
