#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/edge/endvertex.h"
#include "render_meshmod/registry.h"

static void const* EdgeEndVertexDefaultData() {
	static MeshMod_EdgeEndVertex invalid = { 0 };
	return &invalid;
}

static bool EdgeEndVertexEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_EdgeEndVertex const* a = (MeshMod_EdgeEndVertex const*)va;
	MeshMod_EdgeEndVertex const* b = (MeshMod_EdgeEndVertex const*)vb;
	return	Handle_HandleEqual64(a->handle, b->handle);
}

static char const* EdgeEndVertexDescription() {
	return"Edge End Verex";
}

AL2O3_EXTERN_C void MeshMod_EdgeEndVertexAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
			.defaultDataFunc = &EdgeEndVertexDefaultData,
			.equalFunc = &EdgeEndVertexEqual,
			.destroyFunc = NULL,
			.descriptionFunc = &EdgeEndVertexDescription,
			.distanceFunc = NULL
	};

	static MeshMod_RegistryEdgeFunctionTable EdgeFunctionTable = {
			0
	};

	MeshMod_RegistryAddType(handle,
													MeshMod_EdgeEndVertexTag.tag,
			sizeof(MeshMod_EdgeEndVertex),
			&CommonFunctionTable,
			&EdgeFunctionTable);
}


