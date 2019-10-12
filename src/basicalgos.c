#include "render_meshmod/basicalgos.h"
#include "render_meshmod/data/aabb.h"

AL2O3_EXTERN_C MeshMod_DataAabb3F const* MeshMod_MeshComputeExtents3F(MeshMod_MeshHandle handle, MeshMod_Tag tag) {
	MeshMod_DataContainerHandle dc = MeshMod_MeshGetVertices(handle);
	if (MeshMod_DataContainerExists(dc, tag) == false) { return NULL; }
	uint64_t hash = MeshMod_DataContainerHash(dc, tag);
	MeshMod_Tag dataTag = MESHMOD_HASHTAG(tag);
	MeshMod_DataAabb3F *aabb;
	if (MeshMod_MeshHasData(handle, dataTag) == false) {
		aabb = (MeshMod_DataAabb3F*) MeshMod_MeshAddDataZeroed(handle, dataTag, sizeof(MeshMod_DataAabb3F));
		aabb->dataTag = dataTag;
	}
	else {
		aabb = (MeshMod_DataAabb3F*)MeshMod_MeshGetData(handle, dataTag);
		ASSERT(aabb->dataTag == dataTag);
	}
	if (hash == 0 || (aabb->hash != hash)) {
		aabb->aabb = Math_Aabb3FCreateInvalid();
		aabb->hash = hash;
		CADT_VectorHandle v = MeshMod_DataContainerConstLookup(dc, tag);
		for (size_t i = 0; i < CADT_VectorSize(v); ++i) {
			Math_Vec3F d = *(Math_Vec3F*)CADT_VectorAt(v, i);
			Math_ExpandAabb3F(&aabb->aabb, d);
		}
	}

	return aabb;
}

AL2O3_EXTERN_C MeshMod_DataAabb2F const* MeshMod_MeshComputeExtents2F(MeshMod_MeshHandle handle, MeshMod_Tag tag) {

}
