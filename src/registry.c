#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cadt/vector.h"
#include "al2o3_cadt/dict.h"
#include "render_meshmod/registry.h"
#include "registry.h"
#include <stddef.h>

extern void MeshMod_AddVertexDefaultsToTagRegistry(MeshMod_RegistryHandle handle);
extern void MeshMod_AddEdgeDefaultsToTagRegistry(MeshMod_RegistryHandle handle);
extern void MeshMod_AddPolygonDefaultsToTagRegistry(MeshMod_RegistryHandle handle);
static uint64_t const UserDataMask = 0x00FF0000000000FF;


static MeshMod_RegistryItem* LookupRegistryItem(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	ASSERT(MeshMod_RegistryHandleIsValid(handle));
	MeshMod_Registry* reg = MeshMod_RegistryHandleToPtr(handle);

	ASSERT(CADT_DictU64KeyExists(reg->tagDictionary, tag & ~UserDataMask));

	uint64_t index = 0;
	bool okayLookupReg = CADT_DictU64Lookup(reg->tagDictionary, tag & ~UserDataMask, &index);
	if (!okayLookupReg)
		return 0;

	ASSERT(index < CADT_VectorSize(reg->registry));
	return CADT_VectorAt(reg->registry, index);
}

AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_RegistryCreate() {
	MeshMod_RegistryHandle handle = MeshMod_RegistryHandleAlloc();
	if(!MeshMod_RegistryHandleIsValid(handle)) {
		MeshMod_RegistryHandle invalid = {0};
		return invalid;
	}

	MeshMod_Registry* reg = MeshMod_RegistryHandleToPtr(handle);
	reg->registry = CADT_VectorCreate(sizeof(MeshMod_RegistryItem));
	if (reg->registry == NULL) {
		MeshMod_RegistryDestroy(handle);
		MeshMod_RegistryHandle invalid = {0};
		return invalid;
	}
	reg->tagDictionary = CADT_DictU64Create();
	if (reg->tagDictionary == NULL) {
		MeshMod_RegistryDestroy(handle);
		MeshMod_RegistryHandle invalid = {0};
		return invalid;
	}

	return handle;
}

AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_RegistryCreateWithDefaults() {
	MeshMod_RegistryHandle handle = MeshMod_RegistryCreate();

	MeshMod_AddVertexDefaultsToTagRegistry(handle);
	MeshMod_AddEdgeDefaultsToTagRegistry(handle);
	MeshMod_AddPolygonDefaultsToTagRegistry(handle);

	return handle;
}

AL2O3_EXTERN_C void MeshMod_RegistryDestroy(MeshMod_RegistryHandle handle) {
	ASSERT(MeshMod_RegistryHandleIsValid(handle));
	MeshMod_Registry* reg = MeshMod_RegistryHandleToPtr(handle);

	CADT_DictU64Destroy(reg->tagDictionary);
	CADT_VectorDestroy(reg->registry);
	MeshMod_RegistryHandleRelease(handle);
}
AL2O3_EXTERN_C MeshMod_RegistryHandle MeshMod_RegistryClone(MeshMod_RegistryHandle handle) {
	ASSERT(MeshMod_RegistryHandleIsValid(handle));
	MeshMod_Registry* src = MeshMod_RegistryHandleToPtr(handle);

	MeshMod_RegistryHandle newHandle = MeshMod_RegistryHandleAlloc();
	MeshMod_Registry* dst = MeshMod_RegistryHandleToPtr(newHandle);

	dst->tagDictionary = CADT_DictU64Clone(src->tagDictionary);
	dst->registry = CADT_VectorClone(src->registry);

	return newHandle;
}

AL2O3_EXTERN_C void MeshMod_RegistryAddType(	MeshMod_RegistryHandle handle,
																							MeshMod_Tag tag,
																							size_t elementSize,
																							MeshMod_RegistryCommonFunctionTable* commonFunctionTable,
																							void* functionTable) {
	ASSERT(MeshMod_RegistryHandleIsValid(handle));
	MeshMod_Registry* reg = MeshMod_RegistryHandleToPtr(handle);

	MeshMod_RegistryItem item;
	item.type = (MeshMod_Type)(tag >> 56);
	item.elementSize = elementSize;
	item.commonFunctionTable = commonFunctionTable;
	item.functionTable = functionTable;

	size_t index = CADT_VectorPushElement(reg->registry, &item);
	bool okayAddType = CADT_DictU64Add(reg->tagDictionary, tag & ~UserDataMask, (uint64_t)index);
	ASSERT(okayAddType);
}

AL2O3_EXTERN_C bool MeshMod_RegistryExists(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	ASSERT(MeshMod_RegistryHandleIsValid(handle));
	MeshMod_Registry* reg = MeshMod_RegistryHandleToPtr(handle);

	return CADT_DictU64KeyExists(reg->tagDictionary, tag & ~UserDataMask);
}

AL2O3_EXTERN_C size_t MeshMod_RegistryElementSize(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	MeshMod_RegistryItem* item = LookupRegistryItem(handle, tag);
	return item->elementSize;
}

AL2O3_EXTERN_C MeshMod_Type MeshMod_RegistryType(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	MeshMod_RegistryItem* item = LookupRegistryItem(handle, tag);
	return item->type;
}

AL2O3_EXTERN_C char const* MeshMod_RegistryDescription(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	MeshMod_RegistryItem* item = LookupRegistryItem(handle, tag);
	if (item->commonFunctionTable->descriptionFunc)
		return item->commonFunctionTable->descriptionFunc();
	else return "No Description";
}

AL2O3_EXTERN_C void const* MeshMod_RegistryDefaultData(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	MeshMod_RegistryItem* item = LookupRegistryItem(handle, tag);
	return item->commonFunctionTable->defaultDataFunc();
}

AL2O3_EXTERN_C bool MeshMod_RegistryIsDefaultData(MeshMod_RegistryHandle handle, MeshMod_Tag tag, void const* testData) {
	MeshMod_RegistryItem* item = LookupRegistryItem(handle, tag);
	return memcmp(testData, item->commonFunctionTable->defaultDataFunc(), item->elementSize) == 0;
}
AL2O3_EXTERN_C bool MeshMod_RegistryHasDestroy(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	MeshMod_RegistryItem* item = LookupRegistryItem(handle, tag);
	return item->commonFunctionTable->destroyFunc != NULL;
}

AL2O3_EXTERN_C MeshMod_RegistryCommonFunctionTable* MeshMod_RegistryGetCommonFunctionTable(MeshMod_RegistryHandle handle, MeshMod_Tag tag) {
	MeshMod_RegistryItem* item = LookupRegistryItem(handle, tag);
	return item->commonFunctionTable;
}

AL2O3_EXTERN_C void* MeshMod_RegistryFunctionTable(MeshMod_RegistryHandle handle, MeshMod_Tag tag, MeshMod_Type type) {
	MeshMod_RegistryItem* item = LookupRegistryItem(handle, tag);
	ASSERT(type == item->type);
	return item->functionTable;
}