#include "al2o3_platform/platform.h"
#include "gfx_meshmod/polygon/tribrep.h"
#include "gfx_meshmod/registry.h"

static void* PolygonTriBRepDefaultData() {
	static MeshMod_PolygonTriBRep invalid = { MeshMod_InvalidIndex, MeshMod_InvalidIndex, MeshMod_InvalidIndex };
	return &invalid;
}

static bool PolygonTriBRepEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_PolygonTriBRep const* a = (MeshMod_PolygonTriBRep const*)va;
	MeshMod_PolygonTriBRep const* b = (MeshMod_PolygonTriBRep const*)vb;
	return	a->edge[0] == b->edge[0] &&
					a->edge[1] == b->edge[1] &&
					a->edge[2] == b->edge[2];
}

static AL2O3_EXTERN_C char const* PolygonTriBRepDescription() {
	return "Triangle Boundary Rep";
}

AL2O3_EXTERN_C void MeshMod_PolygonTriBRepAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		&PolygonTriBRepDefaultData,
		&PolygonTriBRepEqual,
		NULL,
		&PolygonTriBRepDescription
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
