#include "al2o3_platform/platform.h"
#include "render_meshmod/polygon/line.h"
#include "render_meshmod/registry.h"

static void const* PolygonLineDefaultData() {
	static MeshMod_PolygonLine const invalid = {0 };
	return &invalid;
}

static bool PolygonLineEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_PolygonLine const* a = (MeshMod_PolygonLine const*)va;
	MeshMod_PolygonLine const* b = (MeshMod_PolygonLine const*)vb;
	return	Handle_HandleEqual64(a->edge.handle, b->edge.handle);
}

static AL2O3_EXTERN_C char const* PolygonLineDescription() {
	return "Line polygon";
}

AL2O3_EXTERN_C void MeshMod_PolygonLineAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
			.defaultDataFunc = &PolygonLineDefaultData,
			.equalFunc = &PolygonLineEqual,
			.destroyFunc = NULL,
			.descriptionFunc = &PolygonLineDescription,
			.distanceFunc = NULL
	};
	static MeshMod_RegistryPolygonFunctionTable PolygonFunctionTable = {
			0
	};

	MeshMod_RegistryAddType(	handle,
														MeshMod_PolygonLineTag.tag,
														sizeof(MeshMod_PolygonLine),
														&CommonFunctionTable,
														&PolygonFunctionTable);
}
