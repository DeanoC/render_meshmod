#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/vertex/ring.h"
#include "render_meshmod/registry.h"

static void const* VertexRingDefaultData() {
	static MeshMod_VertexRing invalid = { 0 };
	return &invalid;
}

static bool VertexRingEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_VertexRing const* a = (MeshMod_VertexRing const*)va;
	MeshMod_VertexRing const* b = (MeshMod_VertexRing const*)vb;
	return	a->next.handle.handle == b->next.handle.handle;
}

static char const* VertexRingDescription() {
	return"Vertex Handle Ring";
}

AL2O3_EXTERN_C void MeshMod_VertexRingAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
			.defaultDataFunc = &VertexRingDefaultData,
			.equalFunc = &VertexRingEqual,
			.destroyFunc = NULL,
			.descriptionFunc = &VertexRingDescription,
			.distanceFunc = NULL
	};

	static MeshMod_RegistryVertexFunctionTable VertexFunctionTable = {
			0
	};

	MeshMod_RegistryAddType(handle,
													MeshMod_VertexRingTag.tag,
													sizeof(MeshMod_VertexRing),
													&CommonFunctionTable,
													&VertexFunctionTable);
}


