#include "al2o3_platform/platform.h"
#include "al2o3_memory/memory.h"
#include "gfx_meshmod/cvector.h"
#include "gfx_meshmod/cdict.h"
#include "gfx_meshmod/tag.h"
#include "gfx_meshmod/cbagofvectors.h"

typedef struct MeshMod_CBagOfVectors {
	MeshMod_CDictU64Handle tagDictHandle;
	// destroy could be much faster we hold a vector of vectors and just
	// store the index in the dictionary.
} MeshMod_CBagOfVectors;


AL2O3_EXTERN_C MeshMod_CBagOfVectorsHandle MeshMod_CBagOfVectorsCreate() {
	ASSERT(sizeof(MeshMod_CVectorHandle) <= sizeof(uint64_t));

	MeshMod_CBagOfVectors* tvec = (MeshMod_CBagOfVectors*) MEMORY_CALLOC(1, sizeof(MeshMod_CBagOfVectors));
	if(tvec == NULL) return NULL;
	tvec->tagDictHandle = MeshMod_CDictU64Create();
	if(tvec->tagDictHandle == NULL) {
		MEMORY_FREE(tvec);
		return NULL;
	}

	return tvec;
}

AL2O3_EXTERN_C void MeshMod_CBagOfVectorsDestroy(MeshMod_CBagOfVectorsHandle handle) {
	ASSERT(handle);
	MeshMod_CBagOfVectors* tvec = (MeshMod_CBagOfVectors*) handle;

	// note this is v.slow for large size dictionary
	// GetByIndex linear walks the dictionary for each index it gets
	for(size_t i = 0; i < MeshMod_CDictU64Size(tvec->tagDictHandle);i++) {
		MeshMod_CVectorHandle vh = (MeshMod_CVectorHandle)MeshMod_CDictU64GetByIndex(tvec->tagDictHandle, i);
		MeshMod_CVectorDestroy(vh);
	}

	MeshMod_CDictU64Destroy(tvec->tagDictHandle);
	MEMORY_FREE(tvec);
}

AL2O3_EXTERN_C MeshMod_CVectorHandle MeshMod_CBagOfVectorsAdd(MeshMod_CBagOfVectorsHandle handle, uint64_t key, size_t elementSize) {
	ASSERT(MeshMod_CBagOfVectorsKeyExists(handle, key) == false);

	MeshMod_CBagOfVectors* tvec = (MeshMod_CBagOfVectors*) handle;
	ASSERT(tvec->tagDictHandle);
	MeshMod_CVectorHandle vh = MeshMod_CVectorCreate(elementSize);
	bool okay = MeshMod_CDictU64Add(tvec->tagDictHandle, key, (uint64_t)vh);
	if (okay) return vh;
	else {
		MeshMod_CVectorDestroy(vh);
		return NULL;
	}

}

AL2O3_EXTERN_C void MeshMod_CBagOfVectorsOwnVector(MeshMod_CBagOfVectorsHandle handle, uint64_t key, MeshMod_CVectorHandle vector) {
	ASSERT(MeshMod_CBagOfVectorsKeyExists(handle, key) == false);

	MeshMod_CBagOfVectors* tvec = (MeshMod_CBagOfVectors*) handle;
	MeshMod_CDictU64Add(tvec->tagDictHandle, key, (uint64_t)vector);
}

AL2O3_EXTERN_C void MeshMod_CBagOfVectorsRemove(MeshMod_CBagOfVectorsHandle handle, uint64_t key) {
	ASSERT(MeshMod_CBagOfVectorsKeyExists(handle, key) == true);
	MeshMod_CBagOfVectors* tvec = (MeshMod_CBagOfVectors*) handle;
	MeshMod_CVectorHandle vh = MeshMod_CBagOfVectorsGet(handle, key);
	MeshMod_CDictU64Remove(tvec->tagDictHandle, key);
	MeshMod_CVectorDestroy(vh);
}

AL2O3_EXTERN_C bool MeshMod_CBagOfVectorsKeyExists(MeshMod_CBagOfVectorsHandle handle, uint64_t key) {
	ASSERT(handle);
	MeshMod_CBagOfVectors* tvec = (MeshMod_CBagOfVectors*) handle;
	return MeshMod_CDictU64KeyExists(tvec->tagDictHandle, key);
}

AL2O3_EXTERN_C size_t MeshMod_CBagOfVectorsKeyCount(MeshMod_CBagOfVectorsHandle handle) {
	ASSERT(handle);
	MeshMod_CBagOfVectors* tvec = (MeshMod_CBagOfVectors*) handle;
	return MeshMod_CDictU64Size(tvec->tagDictHandle);
}

AL2O3_EXTERN_C uint64_t MeshMod_CBagOfVectorsGetKey(MeshMod_CBagOfVectorsHandle handle, size_t index) {
	ASSERT(handle);
	MeshMod_CBagOfVectors* tvec = (MeshMod_CBagOfVectors*) handle;
	return MeshMod_CDictU64GetByIndex(tvec->tagDictHandle, index);
}

AL2O3_EXTERN_C MeshMod_CVectorHandle MeshMod_CBagOfVectorsGet(MeshMod_CBagOfVectorsHandle handle, uint64_t key) {
	ASSERT(MeshMod_CBagOfVectorsKeyExists(handle, key) == true);
	MeshMod_CBagOfVectors* tvec = (MeshMod_CBagOfVectors*) handle;
	return (MeshMod_CVectorHandle)MeshMod_CDictU64Get(tvec->tagDictHandle, key);
}
