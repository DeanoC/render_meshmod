#include "al2o3_platform/platform.h"
#include "gfx_meshmod/registry.h"

#define REGISTER_TYPE(x) \
	AL2O3_EXTERN_C void MeshMod_Vertex##x##AddToRegistry(MeshMod_RegistryHandle handle); \
	MeshMod_Vertex##x##AddToRegistry(handle);

void MeshMod_AddVertexDefaultsToTagRegistry(MeshMod_RegistryHandle handle) {
	REGISTER_TYPE(F);
	REGISTER_TYPE(D);
	REGISTER_TYPE(Vec2F);
	REGISTER_TYPE(Vec3F);
	REGISTER_TYPE(Vec4F);
	REGISTER_TYPE(Vec2D);
	REGISTER_TYPE(Vec3D);
	REGISTER_TYPE(Vec4D);
	REGISTER_TYPE(Position);
	REGISTER_TYPE(Normal);
	REGISTER_TYPE(Uv);
}