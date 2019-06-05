#include "al2o3_platform/platform.h"
#include "gfx_meshmod/vertex/basicdata.h"
#include "gfx_meshmod/vertex/normal.h"
#include "gfx_meshmod/registry.h"
static void* VertexNormalDefaultData() {
	static Math_Vec3F_t nan = { NAN, NAN, NAN };
	return &nan;
}

static char const* VertexNormalDescription() {
	return "Vertex Normal";
}

AL2O3_EXTERN_C void MeshMod_VertexNormalAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		&VertexNormalDefaultData,
		&MeshMod_VertexVec3FEqual,
		NULL,
		&VertexNormalDescription
	};

	static MeshMod_RegistryVertexFunctionTable VertexFunctionTable = {
		&MeshMod_VertexVec3FInterpolate1D, // TODO specilize to spherical interpolation
		&MeshMod_VertexVec3FInterpolate2D // TODO specilize to spherical interpolation
	};

	MeshMod_RegistryAddType(handle,
		MeshMod_VertexNormalTag,
		sizeof(MeshMod_VertexNormal),
		&CommonFunctionTable,
		&VertexFunctionTable);
}

