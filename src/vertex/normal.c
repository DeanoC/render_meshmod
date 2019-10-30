#include "al2o3_platform/platform.h"
#include "render_meshmod/vertex/basicdata.h"
#include "render_meshmod/vertex/normal.h"
#include "render_meshmod/registry.h"

// borrow from basic data
AL2O3_EXTERN_C bool MeshMod_VertexVec3FEqual(void const* va, void const* vb, float const epsilon);
AL2O3_EXTERN_C double MeshMod_VertexVec3FDistance(void const* va, void const* vb);
AL2O3_EXTERN_C void MeshMod_VertexVec3FInterpolate1D(void const* va, void const* vb,void* vr, float const t);
AL2O3_EXTERN_C void MeshMod_VertexVec3FInterpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);

static void const* VertexNormalDefaultData() {
	static Math_Vec3F nan = { NAN, NAN, NAN };
	return &nan;
}

static char const* VertexNormalDescription() {
	return "Vertex Normal";
}

AL2O3_EXTERN_C void MeshMod_VertexNormalAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		.defaultDataFunc = &VertexNormalDefaultData,
		.equalFunc = &MeshMod_VertexVec3FEqual,
		.destroyFunc = NULL,
		.descriptionFunc = &VertexNormalDescription,
		.distanceFunc = &MeshMod_VertexVec3FDistance,
	};

	static MeshMod_RegistryVertexFunctionTable VertexFunctionTable = {
		&MeshMod_VertexVec3FInterpolate1D, // TODO specilize to spherical interpolation
		&MeshMod_VertexVec3FInterpolate2D // TODO specilize to spherical interpolation
	};

	MeshMod_RegistryAddType(handle,
		MeshMod_VertexNormalTag.tag,
		sizeof(MeshMod_VertexNormal),
		&CommonFunctionTable,
		&VertexFunctionTable);
}

