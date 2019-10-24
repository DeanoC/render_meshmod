#include "al2o3_platform/platform.h"
#include "render_meshmod/polygon/quadbrep.h"
#include "render_meshmod/registry.h"

static void const* PolygonQuadBRepDefaultData() {
	static MeshMod_PolygonQuadBRep const invalid = { 0 };
	return &invalid;
}

static bool PolygonQuadBRepEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_PolygonQuadBRep const* a = (MeshMod_PolygonQuadBRep const*)va;
	MeshMod_PolygonQuadBRep const* b = (MeshMod_PolygonQuadBRep const*)vb;
	return
			a->edge[0].handle.handle == b->edge[0].handle.handle &&
			a->edge[1].handle.handle == b->edge[1].handle.handle &&
			a->edge[2].handle.handle == b->edge[2].handle.handle &&
			a->edge[3].handle.handle == b->edge[3].handle.handle;
}

static AL2O3_EXTERN_C char const* PolygonQuadBRepDescription() {
	return "Quad Boundary Rep";
}

AL2O3_EXTERN_C void MeshMod_PolygonQuadBRepAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
			.defaultDataFunc = &PolygonQuadBRepDefaultData,
			.equalFunc = &PolygonQuadBRepEqual,
			.destroyFunc = NULL,
			.descriptionFunc = &PolygonQuadBRepDescription,
			.distanceFunc = NULL
	};
	static MeshMod_RegistryPolygonFunctionTable PolygonFunctionTable = {
			0
	};

	MeshMod_RegistryAddType(	handle,
														MeshMod_PolygonQuadBRepTag,
														sizeof(MeshMod_PolygonQuadBRep),
														&CommonFunctionTable,
														&PolygonFunctionTable);
}


