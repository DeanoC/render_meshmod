#include "al2o3_platform/platform.h"
#include "gfx_meshmod/meshmod.h"
#include "gfx_meshmod/edge/halfedge.h"
#include "gfx_meshmod/registry.h"

static void* EdgeHalfEdgeDefaultData() {
	static MeshMod_EdgeHalfEdge invalid = { MeshMod_InvalidIndex, MeshMod_InvalidIndex };
	return &invalid;
}

static bool EdgeHalfEdgeEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_EdgeHalfEdge const* a = (MeshMod_EdgeHalfEdge const*)va;
	MeshMod_EdgeHalfEdge const* b = (MeshMod_EdgeHalfEdge const*)vb;
	return	a->vertex == b->vertex && a->polygon == b->polygon;
}

static char const* EdgeHalfEdgeDescription() {
	return"Half edge linking polygon to a vertex";
}

AL2O3_EXTERN_C void MeshMod_EdgeHalfEdgeAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		&EdgeHalfEdgeDefaultData,
		&EdgeHalfEdgeEqual,
		NULL,
		&EdgeHalfEdgeDescription
		};

	static MeshMod_RegistryEdgeFunctionTable EdgeFunctionTable = {
		0
	};

	MeshMod_RegistryAddEdgeType(handle,
		MeshMod_EdgeHalfEdgeTag,
		sizeof(MeshMod_EdgeHalfEdge),
		&CommonFunctionTable,
		&EdgeFunctionTable);
}
