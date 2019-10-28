#include "al2o3_platform/platform.h"
#include "render_meshmod/vertex/basicdata.h"
#include "render_meshmod/vertex/uv.h"
#include "render_meshmod/registry.h"

// borrow from basic data
AL2O3_EXTERN_C bool MeshMod_VertexVec2FEqual(void const* va, void const* vb, float const epsilon);
AL2O3_EXTERN_C double MeshMod_VertexVec2FDistance(void const* va, void const* vb);
AL2O3_EXTERN_C void MeshMod_VertexVec2FInterpolate1D(void const* va, void const* vb,void* vr, float const t);
AL2O3_EXTERN_C void MeshMod_VertexVec2FInterpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);

static void const* VertexUvDefaultData() {
	static Math_Vec2F nan = { NAN, NAN };
	return &nan;
}

static char const* VertexUvDescription() {
	return "Vertex Uv";
}

AL2O3_EXTERN_C void MeshMod_VertexUvAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		.defaultDataFunc = &VertexUvDefaultData,
		.equalFunc = &MeshMod_VertexVec2FEqual,
		.destroyFunc = NULL,
		.descriptionFunc = &VertexUvDescription,
		.distanceFunc = &MeshMod_VertexVec2FDistance,
	};

	static MeshMod_RegistryVertexFunctionTable VertexFunctionTable = {
		&MeshMod_VertexVec2FInterpolate1D,
		&MeshMod_VertexVec2FInterpolate2D
	};

	MeshMod_RegistryAddType(handle,
		MeshMod_VertexUvTag.tag,
		sizeof(MeshMod_VertexUv),
		&CommonFunctionTable,
		&VertexFunctionTable);
}

