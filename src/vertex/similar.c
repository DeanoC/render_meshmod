#include "al2o3_platform/platform.h"
#include "render_meshmod/vertex/similar.h"
#include "render_meshmod/registry.h"
static void const* VertexSimilarDefaultData() {
	static MeshMod_VertexHandle invalid = {0};
	return &invalid;
}

static char const* VertexSimilarDescription() {
	return "Vertex Similar Ring";
}

static bool VertexSimilarEqual(void const* va, void const* vb, float const epsilon) {

	MeshMod_VertexSimilar const* a = (MeshMod_VertexSimilar const*)va;
	MeshMod_VertexSimilar const* b = (MeshMod_VertexSimilar const*)vb;
	return a->handle.handle == b->handle.handle;
}; 

static double VertexSimilarDistance(void const* va, void const* vb) {
	MeshMod_VertexSimilar const* a = (MeshMod_VertexSimilar const*)va;
	MeshMod_VertexSimilar const* b = (MeshMod_VertexSimilar const*)vb;
	return (double)a->handle.handle == (double)b->handle.handle;
};

AL2O3_EXTERN_C void MeshMod_VertexSimilarAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		.defaultDataFunc = &VertexSimilarDefaultData,
		.equalFunc = &VertexSimilarEqual,
		.destroyFunc = NULL,
		.descriptionFunc = &VertexSimilarDescription,
		.distanceFunc = &VertexSimilarDistance
	};

	static MeshMod_RegistryVertexFunctionTable VertexFunctionTable = {
		NULL,
		NULL
	};

	MeshMod_RegistryAddType(handle,
		MeshMod_VertexSimilarTag.tag,
		sizeof(MeshMod_VertexSimilar),
		&CommonFunctionTable,
		&VertexFunctionTable);
}

