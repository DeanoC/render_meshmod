#include "al2o3_platform/platform.h"
#include "gfx_meshmod/vertex/similar.h"
#include "gfx_meshmod/registry.h"
static void* VertexSimilarDefaultData() {
	static MeshMod_VertexIndex invalid = MeshMod_InvalidIndex;
	return &invalid;
}

static char const* VertexSimilarDescription() {
	return "Vertex Similar Ring";
}

static bool VertexSimilarEqual(void const* va, void const* vb, float const epsilon) {
	
	MeshMod_VertexSimilar const* a = (MeshMod_VertexSimilar const*)va; 
	MeshMod_VertexSimilar const* b = (MeshMod_VertexSimilar const*)vb; 
	return *a == *b; 
}; 

static double VertexSimilarDistance(void const* va, void const* vb) {
	MeshMod_VertexSimilar const* a = (MeshMod_VertexSimilar const*)va;
	MeshMod_VertexSimilar const* b = (MeshMod_VertexSimilar const*)vb;
	return (double)*a == (double)*b;
};
static bool VertexSimilarIsTransitory() {
	return true;
}
AL2O3_EXTERN_C void MeshMod_VertexSimilarAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		&VertexSimilarDefaultData,
		&VertexSimilarEqual,
		NULL,
		&VertexSimilarDescription,
		&VertexSimilarIsTransitory,
		&VertexSimilarDistance
	};

	static MeshMod_RegistryVertexFunctionTable VertexFunctionTable = {
		NULL,
		NULL
	};

	MeshMod_RegistryAddType(handle,
		MeshMod_VertexSimilarTag,
		sizeof(MeshMod_VertexSimilar),
		&CommonFunctionTable,
		&VertexFunctionTable);
}

