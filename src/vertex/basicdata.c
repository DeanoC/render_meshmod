#include "al2o3_platform/platform.h"
#include "render_meshmod/vertex/basicdata.h"
#include "render_meshmod/registry.h"

#define VT_REAL(postfix, type) \
static char const* Vertex##postfix##Description() { \
	return "Vertex"#postfix; \
}; \
void const* MeshMod_Vertex##postfix##DefaultData() { \
	static type const nan = NAN; \
	return &nan; \
}; \
AL2O3_EXTERN_C bool MeshMod_Vertex##postfix##Equal(void const* va, void const* vb, float const epsilon) { \
	MeshMod_Vertex##postfix const* a = (MeshMod_Vertex##postfix const*)va; \
	MeshMod_Vertex##postfix const* b = (MeshMod_Vertex##postfix const*)vb; \
	return Math_ApproxEqual##postfix(*a, *b, epsilon); \
}; \
AL2O3_EXTERN_C double MeshMod_Vertex##postfix##Distance(void const* va, void const* vb) { \
	MeshMod_Vertex##postfix const* a = (MeshMod_Vertex##postfix const*)va; \
	MeshMod_Vertex##postfix const* b = (MeshMod_Vertex##postfix const*)vb; \
	return (double)*a - (double)*b; \
}; \
AL2O3_EXTERN_C void MeshMod_Vertex##postfix##Interpolate1D(void const* va,	void const* vb,	void* vr,	float const t) { \
	MeshMod_Vertex##postfix const* a = (MeshMod_Vertex##postfix const*)va; \
	MeshMod_Vertex##postfix const* b = (MeshMod_Vertex##postfix const*)vb; \
	MeshMod_Vertex##postfix* r = (MeshMod_Vertex##postfix*)vr; \
	*r = Math_Lerp##postfix(*a, *b, t); \
}; \
AL2O3_EXTERN_C void MeshMod_Vertex##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v) { \
	MeshMod_Vertex##postfix const* a = (MeshMod_Vertex##postfix const*)va; \
	MeshMod_Vertex##postfix const* b = (MeshMod_Vertex##postfix const*)vb; \
	MeshMod_Vertex##postfix const* c = (MeshMod_Vertex##postfix const*)vc; \
	MeshMod_Vertex##postfix* r = (MeshMod_Vertex##postfix*)vr; \
	float const w = 1.0f - v - u; \
	*r = (((float)*a) * w) + (((float)*b) * u) + (((float)*c) * v); \
}; \
AL2O3_EXTERN_C void MeshMod_Vertex##postfix##AddToRegistry(MeshMod_RegistryHandle handle) { \
	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = { \
		.defaultDataFunc = &MeshMod_Vertex##postfix##DefaultData, \
		.equalFunc = &MeshMod_Vertex##postfix##Equal, \
		.destroyFunc = NULL,	\
		.descriptionFunc = &Vertex##postfix##Description, \
		.distanceFunc = &MeshMod_Vertex##postfix##Distance, \
	}; \
	static MeshMod_RegistryVertexFunctionTable VertexFunctionTable = { \
		&MeshMod_Vertex##postfix##Interpolate1D, \
		&MeshMod_Vertex##postfix##Interpolate2D \
	}; \
	MeshMod_RegistryAddType(handle, \
		MeshMod_Vertex##postfix##Tag, \
		sizeof(MeshMod_Vertex##postfix), \
		&CommonFunctionTable, \
		&VertexFunctionTable); \
}

static void const* VertexVec4FDefaultData() {
	static Math_Vec4F nan = { NAN, NAN, NAN, NAN };
	return &nan;
}
static void const* VertexVec4DDefaultData() {
	static Math_Vec4D nan = { NAN, NAN, NAN, NAN };
	return &nan;
}
static void const* VertexVec2FDefaultData() { return VertexVec4FDefaultData(); }
static void const* VertexVec3FDefaultData() { return VertexVec4FDefaultData(); }
static void const* VertexVec2DDefaultData() { return VertexVec4DDefaultData(); }
static void const* VertexVec3DDefaultData() { return VertexVec4DDefaultData(); }

#define VT_VECTOR_REAL(postfix, count) \
AL2O3_EXTERN_C bool MeshMod_Vertex##postfix##Equal(void const* va, void const* vb, float const epsilon) { \
	MeshMod_Vertex##postfix const* a = (MeshMod_Vertex##postfix const*)va; \
	MeshMod_Vertex##postfix const* b = (MeshMod_Vertex##postfix const*)vb; \
	return Math_ApproxEqual##postfix(*a, *b, epsilon); \
} \
AL2O3_EXTERN_C double MeshMod_Vertex##postfix##Distance(void const* va, void const* vb) { \
	MeshMod_Vertex##postfix const* a = (MeshMod_Vertex##postfix const*)va; \
	MeshMod_Vertex##postfix const* b = (MeshMod_Vertex##postfix const*)vb; \
	return (double)Math_Dot##postfix(*a, *b); \
} \
AL2O3_EXTERN_C void MeshMod_Vertex##postfix##Interpolate1D(void const* va, void const* vb, void* vr, float const t) { \
	MeshMod_Vertex##postfix const* a = (MeshMod_Vertex##postfix const*)va; \
	MeshMod_Vertex##postfix const* b = (MeshMod_Vertex##postfix const*)vb; \
	MeshMod_Vertex##postfix *r = (MeshMod_Vertex##postfix*)vr; \
	*r = Math_Lerp##postfix(*a, *b, t); \
} \
AL2O3_EXTERN_C void MeshMod_Vertex##postfix##Interpolate2D(void const* va, void const* vb, void const* vc, void* vr, float const u, float const v) { \
	MeshMod_Vertex##postfix const* a = (MeshMod_Vertex##postfix const*)va; \
	MeshMod_Vertex##postfix const* b = (MeshMod_Vertex##postfix const*)vb; \
	MeshMod_Vertex##postfix const* c = (MeshMod_Vertex##postfix const*)vc; \
	MeshMod_Vertex##postfix* r = (MeshMod_Vertex##postfix*)vr; \
	float const w = 1.0f - v - u; \
	for(int i = 0;i < count; ++i) { \
		r->v[i] = ((float)a->v[i] * w) + ((float)b->v[i] * u) + ((float)c->v[i] * v); \
	} \
} \
static char const* Vertex##postfix##Description() { \
	return "Vertex"#postfix; \
} \
AL2O3_EXTERN_C void MeshMod_Vertex##postfix##AddToRegistry(MeshMod_RegistryHandle handle) { \
	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = { \
		.defaultDataFunc = &Vertex##postfix##DefaultData, \
		.equalFunc = &MeshMod_Vertex##postfix##Equal, \
		.destroyFunc = NULL, \
		.descriptionFunc = &Vertex##postfix##Description, \
		.distanceFunc = &MeshMod_Vertex##postfix##Distance, \
	}; \
	static MeshMod_RegistryVertexFunctionTable VertexFunctionTable = { \
		&MeshMod_Vertex##postfix##Interpolate1D, \
		&MeshMod_Vertex##postfix##Interpolate2D \
	}; \
	MeshMod_RegistryAddType(handle, \
		MeshMod_Vertex##postfix##Tag, \
		sizeof(MeshMod_Vertex##postfix), \
		&CommonFunctionTable, \
		&VertexFunctionTable); \
}

VT_REAL(F, float);
VT_REAL(D, double);
VT_VECTOR_REAL(Vec2F, 2);
VT_VECTOR_REAL(Vec3F, 3);
VT_VECTOR_REAL(Vec4F, 4);
VT_VECTOR_REAL(Vec2D, 2);
VT_VECTOR_REAL(Vec3D, 3);
VT_VECTOR_REAL(Vec4D, 4);