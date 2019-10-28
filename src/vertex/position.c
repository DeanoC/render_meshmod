#include "al2o3_platform/platform.h"
#include "render_meshmod/vertex/basicdata.h"
#include "render_meshmod/vertex/position.h"
#include "render_meshmod/registry.h"

// borrow from basic data
AL2O3_EXTERN_C bool MeshMod_VertexVec3FEqual(void const* va, void const* vb, float const epsilon);
AL2O3_EXTERN_C double MeshMod_VertexVec3FDistance(void const* va, void const* vb);
AL2O3_EXTERN_C void MeshMod_VertexVec3FInterpolate1D(void const* va, void const* vb,void* vr, float const t);
AL2O3_EXTERN_C void MeshMod_VertexVec3FInterpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v);

static void const* VertexPositionDefaultData() {
	static Math_Vec3F nan = { NAN, NAN, NAN };
	return &nan;
}

static char const* VertexPositionDescription() {
	return "Vertex Position";
}

AL2O3_EXTERN_C void MeshMod_VertexPositionAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		.defaultDataFunc = &VertexPositionDefaultData,
		.equalFunc = &MeshMod_VertexVec3FEqual,
		.destroyFunc = NULL,
		.descriptionFunc = &VertexPositionDescription,
		.distanceFunc = &MeshMod_VertexVec3FDistance,

	};

	static MeshMod_RegistryVertexFunctionTable VertexFunctionTable = {
		&MeshMod_VertexVec3FInterpolate1D,
		&MeshMod_VertexVec3FInterpolate2D
	};

	MeshMod_RegistryAddType(handle,
		MeshMod_VertexPositionTag.tag,
		sizeof(MeshMod_VertexPosition),
		&CommonFunctionTable,
		&VertexFunctionTable);
}

