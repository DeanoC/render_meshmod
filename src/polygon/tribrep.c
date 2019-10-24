#include "al2o3_platform/platform.h"
#include "render_meshmod/polygon/tribrep.h"
#include "render_meshmod/registry.h"

static void const* PolygonTriBRepDefaultData() {
	static MeshMod_PolygonTriBRep const invalid = {0 };
	return &invalid;
}

static bool PolygonTriBRepEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_PolygonTriBRep const* a = (MeshMod_PolygonTriBRep const*)va;
	MeshMod_PolygonTriBRep const* b = (MeshMod_PolygonTriBRep const*)vb;
	return	a->edge[0].handle.handle == b->edge[0].handle.handle &&
					a->edge[1].handle.handle == b->edge[1].handle.handle &&
					a->edge[2].handle.handle == b->edge[2].handle.handle;
}

static AL2O3_EXTERN_C char const* PolygonTriBRepDescription() {
	return "Triangle Boundary Rep";
}

AL2O3_EXTERN_C void MeshMod_PolygonTriBRepAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		.defaultDataFunc = &PolygonTriBRepDefaultData,
		.equalFunc = &PolygonTriBRepEqual,
		.destroyFunc = NULL,
		.descriptionFunc = &PolygonTriBRepDescription,
		.distanceFunc = NULL
	};
	static MeshMod_RegistryPolygonFunctionTable PolygonFunctionTable = {
		0
	};

	MeshMod_RegistryAddType(	handle,
																	MeshMod_PolygonTriBRepTag,
																	sizeof(MeshMod_PolygonTriBRep),
																	&CommonFunctionTable,
																	&PolygonFunctionTable);
}
