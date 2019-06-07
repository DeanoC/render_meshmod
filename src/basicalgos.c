#include "gfx_meshmod/basicalgos.h"

AL2O3_EXTERN_C MeshMod_DataAabb3F const* MeshMod_MeshComputeExtents2F(MeshMod_MeshHandle handle, MeshMod_Tag tag) {
	MeshMod_DataContainerHandle dc = MeshMod_MeshGetVertices(handle);
	if (MeshMod_DataContainerExists(dc, tag) == false) { return NULL; }
//	bool hasHash = 
	MeshMod_Tag dataTag = MESHMOD_HASHTAG(tag);

}

AL2O3_EXTERN_C MeshMod_DataAabb3F const* MeshMod_MeshComputeExtents3F(MeshMod_MeshHandle handle, MeshMod_Tag tag) {

}
