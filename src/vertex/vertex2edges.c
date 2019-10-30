#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "render_meshmod/vertex/basicdata.h"
#include "render_meshmod/vertex/vertex2edges.h"
#include "render_meshmod/registry.h"

static void const* Vertex2EdgesDefaultData() {
	static MeshMod_Vertex2Edges nan = { 0 };
	return &nan;
}
static bool Vertex2EdgeEqual(void const* av, void const* bv, float const epsilon) {
	MeshMod_Vertex2Edges const* a = (MeshMod_Vertex2Edges const*)av;
	MeshMod_Vertex2Edges const* b = (MeshMod_Vertex2Edges const*)bv;
	if(a->numEdges != b->numEdges) {
		return false;
	}
	for(uint8_t i = 0;i < a->numEdges;++i) {
		if(!Handle_HandleEqual64(a->edges[i].handle, b->edges[i].handle)) {
			return false;
		}
	}
	return true;
}

static char const* Vertex2EdgesDescription() {
	return "Vertex to edges connected to it";
}

static void Vertex2EdgesDestroy(void* element) {
	MeshMod_Vertex2Edges * v2e = (MeshMod_Vertex2Edges*)element;
	if(v2e->numEdges >= MeshMod_Vertex2EdgesMaxEmbedded) {
		MEMORY_FREE(element);
	}
}

AL2O3_EXTERN_C void MeshMod_Vertex2EdgesAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
			.defaultDataFunc = &Vertex2EdgesDefaultData,
			.equalFunc = &Vertex2EdgeEqual,
			.destroyFunc = &Vertex2EdgesDestroy,
			.descriptionFunc = &Vertex2EdgesDescription,
			.distanceFunc = NULL,
	};

	static MeshMod_RegistryEdgeFunctionTable EdgeFunctionTable = {
			0
	};

	MeshMod_RegistryAddType(handle,
													MeshMod_Vertex2EdgesTag.tag,
			sizeof(MeshMod_Vertex2Edges),
			&CommonFunctionTable,
			&EdgeFunctionTable);
}

