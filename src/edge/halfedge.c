#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/edge/halfedge.h"
#include "render_meshmod/registry.h"

static void const* EdgeHalfEdgeDefaultData() {
	static MeshMod_EdgeHalfEdge invalid = { 0 };
	return &invalid;
}

static bool EdgeHalfEdgeEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_EdgeHalfEdge const* a = (MeshMod_EdgeHalfEdge const*)va;
	MeshMod_EdgeHalfEdge const* b = (MeshMod_EdgeHalfEdge const*)vb;
	return	a->vertex.handle.handle == b->vertex.handle.handle && a->polygon.handle.handle == b->polygon.handle.handle;
}

static char const* EdgeHalfEdgeDescription() {
	return"Half edge linking polygon to a vertex";
}

AL2O3_EXTERN_C void MeshMod_EdgeHalfEdgeAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		.defaultDataFunc = &EdgeHalfEdgeDefaultData,
		.equalFunc = &EdgeHalfEdgeEqual,
		.destroyFunc = NULL,
		.descriptionFunc = &EdgeHalfEdgeDescription,
		.distanceFunc = NULL
		};

	static MeshMod_RegistryEdgeFunctionTable EdgeFunctionTable = {
		0
	};

	MeshMod_RegistryAddType(handle,
		MeshMod_EdgeHalfEdgeTag.tag,
		sizeof(MeshMod_EdgeHalfEdge),
		&CommonFunctionTable,
		&EdgeFunctionTable);
}
