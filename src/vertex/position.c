#include "al2o3_platform/platform.h"
#include "render_meshmod/vertex/basicdata.h"
#include "render_meshmod/vertex/position.h"
#include "render_meshmod/registry.h"
static void* VertexPositionDefaultData() {
	static Math_Vec3F nan = { NAN, NAN, NAN };
	return &nan;
}

static char const* VertexPositionDescription() {
	return "Vertex Position";
}

AL2O3_EXTERN_C void MeshMod_VertexPositionAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		&VertexPositionDefaultData,
		&MeshMod_VertexVec3FEqual,
		NULL,
		&VertexPositionDescription,
		NULL,
		&MeshMod_VertexVec3FDistance,

	};

	static MeshMod_RegistryVertexFunctionTable VertexFunctionTable = {
		&MeshMod_VertexVec3FInterpolate1D,
		&MeshMod_VertexVec3FInterpolate2D
	};

	MeshMod_RegistryAddType(handle,
		MeshMod_VertexPositionTag,
		sizeof(MeshMod_VertexPosition),
		&CommonFunctionTable,
		&VertexFunctionTable);
}

