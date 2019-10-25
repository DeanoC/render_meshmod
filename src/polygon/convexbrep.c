#include "al2o3_platform/platform.h"
#include "render_meshmod/polygon/convexbrep.h"
#include "render_meshmod/registry.h"

static void const* PolygonConvexBRepDefaultData() {
	static MeshMod_PolygonConvexBRep const invalid = { 0 };
	return &invalid;
}

static bool PolygonConvexBRepEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_PolygonConvexBRep const* a = (MeshMod_PolygonConvexBRep const*)va;
	MeshMod_PolygonConvexBRep const* b = (MeshMod_PolygonConvexBRep const*)vb;

	if(a->numEdges != b->numEdges) {
		return false;
	}
	for (uint8_t i = 0u; i < a->numEdges;++i) {
		if(a->edge[i].handle.handle != b->edge[i].handle.handle) {
			return false;
		}
	}

	return true;
}

static AL2O3_EXTERN_C char const* PolygonConvexBRepDescription() {
	return "Convex Boundary Rep";
}

AL2O3_EXTERN_C void MeshMod_PolygonConvexBRepAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
			.defaultDataFunc = &PolygonConvexBRepDefaultData,
			.equalFunc = &PolygonConvexBRepEqual,
			.destroyFunc = NULL,
			.descriptionFunc = &PolygonConvexBRepDescription,
			.distanceFunc = NULL
	};
	static MeshMod_RegistryPolygonFunctionTable PolygonFunctionTable = {
			0
	};

	MeshMod_RegistryAddType(	handle,
														MeshMod_PolygonConvexBRepTag.tag,
														sizeof(MeshMod_PolygonConvexBRep),
														&CommonFunctionTable,
														&PolygonFunctionTable);
}




