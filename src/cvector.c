#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "al2o3_cmath/scalar.h"
#include "gfx_meshmod/cvector.h"

typedef struct MeshMod_CVector_t {
	size_t elementSize;
	size_t capacity;
	size_t size;

	void* data;
} MeshMod_CVector_t;

AL2O3_EXTERN_C MeshMod_CVectorHandle MeshMod_CVectorCreate(size_t elementSize) {
	MeshMod_CVector_t* vector = MEMORY_CALLOC(1, sizeof(MeshMod_CVector_t));
	if(vector == NULL) return NULL;
	vector->elementSize = elementSize;
	return vector;
}

AL2O3_EXTERN_C void MeshMod_CVectorDestroy(MeshMod_CVectorHandle handle) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t* vector = (MeshMod_CVector_t*)handle;
	if(vector->data != NULL) {
		MEMORY_FREE(vector->data);
	}
	MEMORY_FREE(vector);
}

AL2O3_EXTERN_C size_t MeshMod_CVectorElementSizeFrom(MeshMod_CVectorHandle handle) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t const* vector = (MeshMod_CVector_t const*)handle;
	return vector->elementSize;
}

AL2O3_EXTERN_C size_t MeshMod_CVectorSizeFrom(MeshMod_CVectorHandle handle) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t const* vector = (MeshMod_CVector_t const*)handle;
	return vector->size;
}

AL2O3_EXTERN_C void MeshMod_CVectorResize(MeshMod_CVectorHandle handle, size_t size) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t * vector = (MeshMod_CVector_t *)handle;

	MeshMod_CVectorReserve(handle, size);
	vector->size = size;
}
AL2O3_EXTERN_C bool MeshMod_CVectorIsEmpty(MeshMod_CVectorHandle handle) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t const* vector = (MeshMod_CVector_t const*)handle;
	return (vector->size == 0);
}
AL2O3_EXTERN_C size_t MeshMod_CVectorCapacityFrom(MeshMod_CVectorHandle handle) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t const* vector = (MeshMod_CVector_t const*)handle;
	return vector->capacity;
}

AL2O3_EXTERN_C void MeshMod_CVectorReserve(MeshMod_CVectorHandle handle, size_t size) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t * vector = (MeshMod_CVector_t *)handle;

	// reserve always grews unless ShrinkToFit
	if(size <= vector->capacity) return;

	void* oldData = vector->data;
	size_t const oldCapacity = (vector->capacity) ? vector->capacity : 1;
	size_t const newCapacity = Math_MaxSizeT(oldCapacity*2, size);
	vector->capacity = newCapacity;
	vector->data = MEMORY_CALLOC(newCapacity, vector->elementSize);
	ASSERT(vector->data);
	if(oldData) {
		memcpy(vector->data, oldData, vector->size * vector->elementSize);
		MEMORY_FREE(oldData);
	}
}

AL2O3_EXTERN_C void* MeshMod_CVectorElementFrom(MeshMod_CVectorHandle handle, size_t index) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t const* vector = (MeshMod_CVector_t const*)handle;
	ASSERT(index < vector->size);
	ASSERT(vector->data);
	return ((uint8_t*)vector->data) + (index * vector->elementSize);
}

AL2O3_EXTERN_C size_t MeshMod_CVectorPushElement(MeshMod_CVectorHandle handle, void* element) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t * vector = (MeshMod_CVector_t *)handle;
	size_t const index = vector->size;
	MeshMod_CVectorResize(handle, vector->size+1);
	ASSERT(index < vector->size);

	memcpy(MeshMod_CVectorElementFrom(handle, index), element, vector->elementSize);
	return index;
}

AL2O3_EXTERN_C void MeshMod_CVectorPopElementFrom(MeshMod_CVectorHandle handle, void* out) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t * vector = (MeshMod_CVector_t *)handle;
	ASSERT(vector->size > 0);
	size_t const index = vector->size-1;
	memcpy(out, MeshMod_CVectorElementFrom(handle, index), vector->elementSize);
	MeshMod_CVectorResize(handle, vector->size-1);
}

AL2O3_EXTERN_C void MeshMod_CVectorPopAndDiscardElementFrom(MeshMod_CVectorHandle handle) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t * vector = (MeshMod_CVector_t *)handle;
	ASSERT(vector->size > 0);
	MeshMod_CVectorResize(handle, vector->size-1);
}

AL2O3_EXTERN_C void* MeshMod_CVectorPeekElementFrom(MeshMod_CVectorHandle handle) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t const* vector = (MeshMod_CVector_t const*)handle;
	ASSERT(vector->size > 0);
	size_t const index = vector->size-1;
	return MeshMod_CVectorElementFrom(handle, index);
}

AL2O3_EXTERN_C void* MeshMod_CVectorDataFrom(MeshMod_CVectorHandle handle) {
	ASSERT(handle != NULL);
	MeshMod_CVector_t const* vector = (MeshMod_CVector_t const*)handle;
	return vector->data;
}
