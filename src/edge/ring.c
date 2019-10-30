#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "render_meshmod/edge/ring.h"
#include "render_meshmod/registry.h"

static void const* EdgeRingDefaultData() {
	static MeshMod_EdgeRing invalid = { 0 };
	return &invalid;
}

static bool EdgeRingEqual(void const* va, void const* vb, float const epsilon)
{
	MeshMod_EdgeRing const* a = (MeshMod_EdgeRing const*)va;
	MeshMod_EdgeRing const* b = (MeshMod_EdgeRing const*)vb;
	return	Handle_HandleEqual64(a->handle, b->handle);
}

static char const* EdgeRingDescription() {
	return"Edge Handle Ring";
}

AL2O3_EXTERN_C void MeshMod_EdgeRingAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
			.defaultDataFunc = &EdgeRingDefaultData,
			.equalFunc = &EdgeRingEqual,
			.destroyFunc = NULL,
			.descriptionFunc = &EdgeRingDescription,
			.distanceFunc = NULL
	};

	static MeshMod_RegistryEdgeFunctionTable EdgeFunctionTable = {
			0
	};

	MeshMod_RegistryAddType(handle,
													MeshMod_EdgeRingTag.tag,
													sizeof(MeshMod_EdgeRing),
													&CommonFunctionTable,
													&EdgeFunctionTable);
}
