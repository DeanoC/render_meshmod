#include "al2o3_platform/platform.h"
#include "gfx_meshmod/vertex/position.h"
#include "gfx_meshmod/registry.h"

static void* VertexPositionDefaultData() {
	static Math_Vec3F_t nan = { NAN, NAN, NAN };
	return &nan;
}

static bool VertexPositionEqual(void const *va, void const *vb, float const epsilon)
{
	MeshMod_VertexPosition const* a = (MeshMod_VertexPosition const*)va;
	MeshMod_VertexPosition const* b = (MeshMod_VertexPosition const*)vb;
	return Math_ApproxEqualVec3F(*a, *b, epsilon);
}

static void VertexPositionInterpolate1D(	void const *va,
																					void const *vb, 
																					void *vr,
																					float const t) {
	MeshMod_VertexPosition const* a = (MeshMod_VertexPosition const*)va;
	MeshMod_VertexPosition const* b = (MeshMod_VertexPosition const*)vb;
	MeshMod_VertexPosition* r = (MeshMod_VertexPosition*)vr;

	*r = Math_LerpVec3F(*a, *b, t);
}

static void VertexPositionInterpolate2D(	void const *va,
																					void const *vb,
																					void const *vc,
																					void *vr,
																					float const u, float const v){
	float const w = 1.0f - v - u;
	MeshMod_VertexPosition const* a = (MeshMod_VertexPosition const*) va;
	MeshMod_VertexPosition const* b = (MeshMod_VertexPosition const*) vb;
	MeshMod_VertexPosition const* c = (MeshMod_VertexPosition const*) vc;
	MeshMod_VertexPosition * r = (MeshMod_VertexPosition *)vr;

	r->x = (a->x * u) + (b->x * v) + (c->x * w);
	r->y = (a->y * u) + (b->y * v) + (c->y * w);
	r->z = (a->z * u) + (b->z * v) + (c->z * w);
}
static char const* VertexPositionDescription() {
	return "Vertex Position";
}

AL2O3_EXTERN_C void MeshMod_VertexPositionAddToRegistry(MeshMod_RegistryHandle handle) {

	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = {
		&VertexPositionDefaultData,
		&VertexPositionEqual,
		NULL,
		&VertexPositionDescription
	};

	static MeshMod_RegistryVertexFunctionTable VertexFunctionTable = {
		&VertexPositionInterpolate1D,
		&VertexPositionInterpolate2D
	};

	MeshMod_RegistryAddVertexType(handle,
		MeshMod_VertexPositionTag,
		sizeof(MeshMod_VertexPosition),
		&CommonFunctionTable,
		&VertexFunctionTable);
}

