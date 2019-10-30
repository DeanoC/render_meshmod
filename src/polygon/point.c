#include "al2o3_platform/platform.h"
#include "render_meshmod/polygon/point.h"
#include "render_meshmod/registry.h"

static void const* PolygonPointDefaultData() {
	static MeshMod_PolygonPoint const invalid = {0 };
	return &invalid;
}

static bool PolygonPointEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_PolygonPoint const* a = (MeshMod_PolygonPoint const*)va;
	MeshMod_PolygonPoint const* b = (MeshMod_PolygonPoint const*)vb;
	return	Handle_HandleEqual64(a->handle, b->handle);
}

static AL2O3_EXTERN_C char const* PolygonPointDescription() {
	return "point polygon";
}

AL2O3_EXTERN_C void MeshMod_PolygonPointAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
			.defaultDataFunc = &PolygonPointDefaultData,
			.equalFunc = &PolygonPointEqual,
			.destroyFunc = NULL,
			.descriptionFunc = &PolygonPointDescription,
			.distanceFunc = NULL
	};
	static MeshMod_RegistryPolygonFunctionTable PolygonFunctionTable = {
			0
	};

	MeshMod_RegistryAddType(	handle,
														MeshMod_PolygonPointTag.tag,
														sizeof(MeshMod_PolygonPoint),
														&CommonFunctionTable,
														&PolygonFunctionTable);
}
