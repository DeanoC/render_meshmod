#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/bagofvectors.h"
#include "render_meshmod/mesh.h"
#include "render_meshmod/registry.h"

#define REGISTER_TYPE(x) \
	AL2O3_EXTERN_C void MeshMod_##x##AddToRegistry(MeshMod_RegistryHandle handle); \
	MeshMod_##x##AddToRegistry(handle);

void MeshMod_AddPolygonDefaultsToTagRegistry(MeshMod_RegistryHandle handle) {
	REGISTER_TYPE(PolygonTriBRep)
}