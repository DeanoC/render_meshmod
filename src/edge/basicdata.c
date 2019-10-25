#include "al2o3_platform/platform.h"
#include "render_meshmod/edge/basicdata.h"
#include "render_meshmod/registry.h"

#define ET_REAL(postfix, type) \
static char const* Edge##postfix##Description() { \
	return "Edge"#postfix; \
}; \
void const* MeshMod_Edge##postfix##DefaultData() { \
	static type const nan = NAN; \
	return &nan; \
}; \
AL2O3_EXTERN_C bool MeshMod_Edge##postfix##Equal(void const* va, void const* vb, float const epsilon) { \
	MeshMod_Edge##postfix const* a = (MeshMod_Edge##postfix const*)va; \
	MeshMod_Edge##postfix const* b = (MeshMod_Edge##postfix const*)vb; \
	return Math_ApproxEqual##postfix(*a, *b, epsilon); \
}; \
AL2O3_EXTERN_C double MeshMod_Edge##postfix##Distance(void const* va, void const* vb) { \
	MeshMod_Edge##postfix const* a = (MeshMod_Edge##postfix const*)va; \
	MeshMod_Edge##postfix const* b = (MeshMod_Edge##postfix const*)vb; \
	return (double)*a - (double)*b; \
}; \
AL2O3_EXTERN_C void MeshMod_Edge##postfix##AddToRegistry(MeshMod_RegistryHandle handle) { \
	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = { \
		.defaultDataFunc = &MeshMod_Edge##postfix##DefaultData, \
		.equalFunc = &MeshMod_Edge##postfix##Equal, \
		.destroyFunc = NULL,	\
		.descriptionFunc = &Edge##postfix##Description, \
		.distanceFunc = &MeshMod_Edge##postfix##Distance, \
	}; \
	static MeshMod_RegistryEdgeFunctionTable EdgeFunctionTable = { \
		0 \
	}; \
	MeshMod_RegistryAddType(handle, \
		MeshMod_Edge##postfix##Tag.tag, \
		sizeof(MeshMod_Edge##postfix), \
		&CommonFunctionTable, \
		&EdgeFunctionTable); \
}

#define ET_UNSIGNED(postfix, type) \
static char const* Edge##postfix##Description() { \
	return "Edge"#postfix; \
}; \
void const* MeshMod_Edge##postfix##DefaultData() { \
	static type const invalid = (type)~0ull; \
	return &invalid; \
}; \
AL2O3_EXTERN_C bool MeshMod_Edge##postfix##Equal(void const* va, void const* vb, float const epsilon) { \
	MeshMod_Edge##postfix const* a = (MeshMod_Edge##postfix const*)va; \
	MeshMod_Edge##postfix const* b = (MeshMod_Edge##postfix const*)vb; \
	return *a == *b; \
}; \
AL2O3_EXTERN_C double MeshMod_Edge##postfix##Distance(void const* va, void const* vb) { \
	MeshMod_Edge##postfix const* a = (MeshMod_Edge##postfix const*)va; \
	MeshMod_Edge##postfix const* b = (MeshMod_Edge##postfix const*)vb; \
	return (double)(*a - *b); \
}; \
AL2O3_EXTERN_C void MeshMod_Edge##postfix##AddToRegistry(MeshMod_RegistryHandle handle) { \
	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = { \
		.defaultDataFunc = &MeshMod_Edge##postfix##DefaultData, \
		.equalFunc = &MeshMod_Edge##postfix##Equal, \
		.destroyFunc = NULL,	\
		.descriptionFunc = &Edge##postfix##Description, \
		.distanceFunc = &MeshMod_Edge##postfix##Distance, \
	}; \
	static MeshMod_RegistryEdgeFunctionTable EdgeFunctionTable = { \
		0 \
	}; \
	MeshMod_RegistryAddType(handle, \
		MeshMod_Edge##postfix##Tag.tag, \
		sizeof(MeshMod_Edge##postfix), \
		&CommonFunctionTable, \
		&EdgeFunctionTable); \
}

static void const* EdgeVec4FDefaultData() {
	static Math_Vec4F nan = { NAN, NAN, NAN, NAN };
	return &nan;
}
static void const* EdgeVec4DDefaultData() {
	static Math_Vec4D nan = { NAN, NAN, NAN, NAN };
	return &nan;
}
static void const* EdgeVec2FDefaultData() { return EdgeVec4FDefaultData(); }
static void const* EdgeVec3FDefaultData() { return EdgeVec4FDefaultData(); }
static void const* EdgeVec2DDefaultData() { return EdgeVec4DDefaultData(); }
static void const* EdgeVec3DDefaultData() { return EdgeVec4DDefaultData(); }

#define ET_VECTOR_REAL(postfix, count) \
AL2O3_EXTERN_C bool MeshMod_Edge##postfix##Equal(void const* va, void const* vb, float const epsilon) { \
	MeshMod_Edge##postfix const* a = (MeshMod_Edge##postfix const*)va; \
	MeshMod_Edge##postfix const* b = (MeshMod_Edge##postfix const*)vb; \
	return Math_ApproxEqual##postfix(*a, *b, epsilon); \
} \
AL2O3_EXTERN_C double MeshMod_Edge##postfix##Distance(void const* va, void const* vb) { \
	MeshMod_Edge##postfix const* a = (MeshMod_Edge##postfix const*)va; \
	MeshMod_Edge##postfix const* b = (MeshMod_Edge##postfix const*)vb; \
	return (double)Math_Dot##postfix(*a, *b); \
} \
static char const* Edge##postfix##Description() { \
	return "Edge"#postfix; \
} \
AL2O3_EXTERN_C void MeshMod_Edge##postfix##AddToRegistry(MeshMod_RegistryHandle handle) { \
	static MeshMod_RegistryCommonFunctionTable CommonFunctionTable = { \
		.defaultDataFunc = &Edge##postfix##DefaultData, \
		.equalFunc = &MeshMod_Edge##postfix##Equal, \
		.destroyFunc = NULL, \
		.descriptionFunc = &Edge##postfix##Description, \
		.distanceFunc = &MeshMod_Edge##postfix##Distance, \
	}; \
	static MeshMod_RegistryEdgeFunctionTable EdgeFunctionTable = { \
	0 \
	}; \
	MeshMod_RegistryAddType(handle, \
		MeshMod_Edge##postfix##Tag.tag, \
		sizeof(MeshMod_Edge##postfix), \
		&CommonFunctionTable, \
		&EdgeFunctionTable); \
}

ET_UNSIGNED(U8, uint8_t)
ET_UNSIGNED(U16, uint16_t)
ET_UNSIGNED(U32, uint32_t)
ET_UNSIGNED(U64, uint64_t)

ET_REAL(F, float);
ET_REAL(D, double);
ET_VECTOR_REAL(Vec2F, 2);
ET_VECTOR_REAL(Vec3F, 3);
ET_VECTOR_REAL(Vec4F, 4);
ET_VECTOR_REAL(Vec2D, 2);
ET_VECTOR_REAL(Vec3D, 3);
ET_VECTOR_REAL(Vec4D, 4);