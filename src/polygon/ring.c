#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/polygon/ring.h"
#include "render_meshmod/registry.h"

static void const* PolygonRingDefaultData() {
	static MeshMod_PolygonRing invalid = { 0 };
	return &invalid;
}

static bool PolygonRingEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_PolygonRing const* a = (MeshMod_PolygonRing const*)va;
	MeshMod_PolygonRing const* b = (MeshMod_PolygonRing const*)vb;
	return	a->next.handle.handle == b->next.handle.handle;
}

static char const* PolygonRingDescription() {
	return"Polygon Handle Ring";
}

AL2O3_EXTERN_C void MeshMod_PolygonRingAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
			.defaultDataFunc = &PolygonRingDefaultData,
			.equalFunc = &PolygonRingEqual,
			.destroyFunc = NULL,
			.descriptionFunc = &PolygonRingDescription,
			.distanceFunc = NULL
	};

	static MeshMod_RegistryPolygonFunctionTable PolygonFunctionTable = {
			0
	};

	MeshMod_RegistryAddType(handle,
													MeshMod_PolygonRingTag.tag,
													sizeof(MeshMod_PolygonRing),
													&CommonFunctionTable,
													&PolygonFunctionTable);
}


