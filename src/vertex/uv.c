#include "al2o3_platform/platform.h"
#include "render_meshmod/vertex/basicdata.h"
#include "render_meshmod/vertex/uv.h"
#include "render_meshmod/registry.h"

static void* VertexUvDefaultData() {
	static Math_Vec2F nan = { NAN, NAN };
	return &nan;
}

static char const* VertexUvDescription() {
	return "Vertex Uv";
}

AL2O3_EXTERN_C void MeshMod_VertexUvAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		&VertexUvDefaultData,
		&MeshMod_VertexVec2FEqual,
		NULL,
		&VertexUvDescription,
		NULL,
		& MeshMod_VertexVec2FDistance,

	};

	static MeshMod_RegistryVertexFunctionTable VertexFunctionTable = {
		&MeshMod_VertexVec2FInterpolate1D,
		&MeshMod_VertexVec2FInterpolate2D
	};

	MeshMod_RegistryAddType(handle,
		MeshMod_VertexUvTag,
		sizeof(MeshMod_VertexUv),
		&CommonFunctionTable,
		&VertexFunctionTable);
}

