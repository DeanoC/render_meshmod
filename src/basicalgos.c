#include "render_meshmod/basicalgos.h"
#include "render_meshmod/data/aabb.h"
#include "render_meshmod/vertex/position.h"
#include "render_meshmod/edge/halfedge.h"
#include "render_meshmod/polygon/tribrep.h"
#include "render_meshmod/polygon/quadbrep.h"
#include "render_meshmod/polygon/convexbrep.h"

AL2O3_EXTERN_C MeshMod_DataAabb3F const *MeshMod_MeshComputeExtents3F(MeshMod_MeshHandle handle, MeshMod_Tag tag) {

	MeshMod_MeshVertexTagEnsure(handle, tag);
	uint64_t hash = MeshMod_MeshVertexTagComputeHash(handle, tag);

	MeshMod_Tag dataTag = MESHMOD_HASHTAG(tag);
	MeshMod_DataAabb3F *aabb;
	if (MeshMod_MeshHasData(handle, dataTag) == false) {
		aabb = (MeshMod_DataAabb3F *) MeshMod_MeshAddDataZeroed(handle, dataTag, sizeof(MeshMod_DataAabb3F));
		aabb->dataTag = dataTag;
	} else {
		aabb = (MeshMod_DataAabb3F *) MeshMod_MeshGetData(handle, dataTag);
		ASSERT(aabb->dataTag == dataTag);
	}

	if (hash == 0 || (aabb->hash != hash)) {
		aabb->aabb = Math_Aabb3FCreateInvalid();
		aabb->hash = hash;
		MeshMod_VertexHandle vhandle = MeshMod_MeshVertexIterate(handle, NULL);
		while (MeshMod_MeshVertexIsValid(handle, vhandle)) {
			Math_Vec3F d = *(Math_Vec3F *) MeshMod_MeshVertexTagHandleToPtr(handle, tag, vhandle);
			Math_ExpandAabb3F(&aabb->aabb, d);

		}
	}

	return aabb;
}

AL2O3_EXTERN_C MeshMod_DataAabb2F const *MeshMod_MeshComputeExtents2F(MeshMod_MeshHandle handle, MeshMod_Tag tag) {

	MeshMod_MeshVertexTagEnsure(handle, tag);
	uint64_t hash = MeshMod_MeshVertexTagComputeHash(handle, tag);

	MeshMod_Tag dataTag = MESHMOD_HASHTAG(tag);
	MeshMod_DataAabb2F *aabb;
	if (MeshMod_MeshHasData(handle, dataTag) == false) {
		aabb = (MeshMod_DataAabb2F *) MeshMod_MeshAddDataZeroed(handle, dataTag, sizeof(MeshMod_DataAabb2F));
		aabb->dataTag = dataTag;
	} else {
		aabb = (MeshMod_DataAabb2F *) MeshMod_MeshGetData(handle, dataTag);
		ASSERT(aabb->dataTag == dataTag);
	}

	if (hash == 0 || (aabb->hash != hash)) {
		aabb->aabb = Math_Aabb2FCreateInvalid();
		aabb->hash = hash;
		MeshMod_VertexHandle vhandle = MeshMod_MeshVertexIterate(handle, NULL);
		while (MeshMod_MeshVertexIsValid(handle, vhandle)) {
			Math_Vec2F d = *(Math_Vec2F *) MeshMod_MeshVertexTagHandleToPtr(handle, tag, vhandle);
			Math_ExpandAabb2F(&aabb->aabb, d);

		}
	}

	return aabb;

}

// convert triangles or quad brep to the general convex type
AL2O3_EXTERN_C void MeshMod_MeshBRepToConvex(MeshMod_MeshHandle handle) {
	// if already valid convex data, nothing to do
	if (MeshMod_MeshVertexTagExists(handle, MeshMod_PolygonConvexBRepTag)) {
		return;
	}

	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonConvexBRepTag);

	// convert triangles if the exist
	if (MeshMod_MeshVertexTagExists(handle, MeshMod_PolygonTriBRepTag)) {
		MeshMod_PolygonHandle triHandle = MeshMod_MeshPolygonTagIterate(handle,
																																		MeshMod_PolygonTriBRepTag,
																																		NULL);
		while (MeshMod_MeshPolygonIsValid(handle, triHandle)) {
			MeshMod_PolygonTriBRep const *triData = (MeshMod_PolygonTriBRep const *) MeshMod_MeshPolygonTagHandleToPtr(handle,
																																																								 MeshMod_PolygonTriBRepTag,
																																																								 triHandle);
			MeshMod_PolygonConvexBRep *convexData = (MeshMod_PolygonConvexBRep *) MeshMod_MeshPolygonTagHandleToPtr(handle,
																																																							MeshMod_PolygonConvexBRepTag,
																																																							triHandle);
			convexData->numEdges = 3;
			convexData->edge[0] = triData->edge[0];
			convexData->edge[1] = triData->edge[1];
			convexData->edge[2] = triData->edge[2];
			triHandle = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonTriBRepTag, &triHandle);
		};

		// remove triangles brep
		MeshMod_MeshPolygonTagRemove(handle, MeshMod_PolygonTriBRepTag);
	}

	// convert quads if the exist
	if (MeshMod_MeshVertexTagExists(handle, MeshMod_PolygonQuadBRepTag)) {
		MeshMod_PolygonHandle quadHandle = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonTriBRepTag, NULL);
		while (MeshMod_MeshPolygonIsValid(handle, quadHandle)) {
			MeshMod_PolygonQuadBRep const *quadData = (MeshMod_PolygonQuadBRep const *) MeshMod_MeshPolygonTagHandleToPtr(
					handle,
					MeshMod_PolygonTriBRepTag,
					quadHandle);
			MeshMod_PolygonConvexBRep *convexData = (MeshMod_PolygonConvexBRep *) MeshMod_MeshPolygonTagHandleToPtr(handle,
																																																							MeshMod_PolygonConvexBRepTag,
																																																							quadHandle);
			convexData->numEdges = 4;
			convexData->edge[0] = quadData->edge[0];
			convexData->edge[1] = quadData->edge[1];
			convexData->edge[2] = quadData->edge[2];
			convexData->edge[3] = quadData->edge[3];
			quadHandle = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonTriBRepTag, &quadHandle);
		};
		// remove quad brep
		MeshMod_MeshPolygonTagRemove(handle, MeshMod_PolygonQuadBRepTag);
	}

	MeshMod_MeshPolygonDataHasChanged(handle);
}

AL2O3_EXTERN_C void MeshMod_MeshTrianglate(MeshMod_MeshHandle handle) {
	// if already valid triangle data, nothing to do
	if (MeshMod_MeshVertexTagExists(handle, MeshMod_PolygonTriBRepTag)) {
		return;
	}

	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonTriBRepTag);

	// convert quads if the exist
	if (MeshMod_MeshVertexTagExists(handle, MeshMod_PolygonQuadBRepTag)) {
		MeshMod_PolygonHandle quadHandle = MeshMod_MeshPolygonTagIterate(handle,
																																		 MeshMod_PolygonQuadBRepTag,
																																		 NULL);

		while (MeshMod_MeshPolygonIsValid(handle, quadHandle)) {
			MeshMod_PolygonQuadBRep const *quadData = (MeshMod_PolygonQuadBRep const *) MeshMod_MeshPolygonTagHandleToPtr(
					handle,
					MeshMod_PolygonTriBRepTag,
					quadHandle);

			MeshMod_PolygonTriBRep *triData0 = (MeshMod_PolygonTriBRep *) MeshMod_MeshPolygonTagHandleToPtr(handle,
																																																			MeshMod_PolygonTriBRepTag,
																																																			quadHandle);
			triData0->edge[0] = quadData->edge[0];
			triData0->edge[1] = quadData->edge[1];
			triData0->edge[2] = quadData->edge[2];
			MeshMod_PolygonHandle newTriHandle = MeshMod_MeshPolygonDuplicate(handle, quadHandle);
			MeshMod_EdgeHandle nedge2 = MeshMod_MeshEdgeDuplicate(handle, quadData->edge[2]);
			MeshMod_EdgeHandle nedge0 = MeshMod_MeshEdgeDuplicate(handle, quadData->edge[0]);

			MeshMod_PolygonTriBRep *triData1 = (MeshMod_PolygonTriBRep *) MeshMod_MeshPolygonTagHandleToPtr(handle,
																																																			MeshMod_PolygonTriBRepTag,
																																																			newTriHandle);
			triData1->edge[0] = nedge2;
			triData1->edge[1] = quadData->edge[3];
			triData1->edge[2] = nedge0;

			MeshMod_EdgeHalfEdge *hedge0 = (MeshMod_EdgeHalfEdge *) MeshMod_MeshEdgeTagHandleToPtr(handle,
																																														 MeshMod_EdgeHalfEdgeTag,
																																														 triData1->edge[0]);
			MeshMod_EdgeHalfEdge *hedge1 = (MeshMod_EdgeHalfEdge *) MeshMod_MeshEdgeTagHandleToPtr(handle,
																																														 MeshMod_EdgeHalfEdgeTag,
																																														 triData1->edge[1]);
			MeshMod_EdgeHalfEdge *hedge2 = (MeshMod_EdgeHalfEdge *) MeshMod_MeshEdgeTagHandleToPtr(handle,
																																														 MeshMod_EdgeHalfEdgeTag,
																																														 triData1->edge[2]);
			hedge0->polygon = newTriHandle;
			hedge1->polygon = newTriHandle;
			hedge2->polygon = newTriHandle;

			quadHandle = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonQuadBRepTag, &quadHandle);
		};

		// remove quad brep
		MeshMod_MeshPolygonTagRemove(handle, MeshMod_PolygonQuadBRepTag);
	}

	// convert convex polygon if the exist
	if (MeshMod_MeshVertexTagExists(handle, MeshMod_PolygonConvexBRepTag)) {
		MeshMod_PolygonHandle convexHandle = MeshMod_MeshPolygonTagIterate(handle,
																																			 MeshMod_PolygonConvexBRepTag,
																																			 NULL);
		while (MeshMod_MeshPolygonIsValid(handle, convexHandle)) {
			MeshMod_PolygonConvexBRep const *convexData =
					(MeshMod_PolygonConvexBRep const *) MeshMod_MeshPolygonTagHandleToPtr(handle,
																																								MeshMod_PolygonConvexBRepTag,
																																								convexHandle);

			// special case the first triangle as it replaces the polygon handle of the original convex
			MeshMod_PolygonTriBRep *triData0 = (MeshMod_PolygonTriBRep *) MeshMod_MeshPolygonTagHandleToPtr(handle,
																																																			MeshMod_PolygonTriBRepTag,
																																																			convexHandle);
			triData0->edge[0] = convexData->edge[0];
			triData0->edge[1] = convexData->edge[1];
			triData0->edge[2] = convexData->edge[2];

			for (uint8_t i = 2; i < convexData->numEdges - 2; i++) {
				MeshMod_PolygonHandle newTriHandle = MeshMod_MeshPolygonDuplicate(handle, convexHandle);

				MeshMod_PolygonTriBRep *triData1 = (MeshMod_PolygonTriBRep *) MeshMod_MeshPolygonTagHandleToPtr(handle,
																																																				MeshMod_PolygonTriBRepTag,
																																																				newTriHandle);
				triData1->edge[0] = MeshMod_MeshEdgeDuplicate(handle, convexData->edge[i]);
				triData1->edge[1] = convexData->edge[i + 1];
				triData1->edge[2] = MeshMod_MeshEdgeDuplicate(handle, convexData->edge[i + 2]);

				MeshMod_EdgeHalfEdge *hedge0 = (MeshMod_EdgeHalfEdge *) MeshMod_MeshEdgeTagHandleToPtr(handle,
																																															 MeshMod_EdgeHalfEdgeTag,
																																															 triData1->edge[0]);
				MeshMod_EdgeHalfEdge *hedge1 = (MeshMod_EdgeHalfEdge *) MeshMod_MeshEdgeTagHandleToPtr(handle,
																																															 MeshMod_EdgeHalfEdgeTag,
																																															 triData1->edge[1]);
				MeshMod_EdgeHalfEdge *hedge2 = (MeshMod_EdgeHalfEdge *) MeshMod_MeshEdgeTagHandleToPtr(handle,
																																															 MeshMod_EdgeHalfEdgeTag,
																																															 triData1->edge[2]);
				hedge0->polygon = newTriHandle;
				hedge1->polygon = newTriHandle;
				hedge2->polygon = newTriHandle;
			}

			convexHandle = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonConvexBRepTag, &convexHandle);
		}

		// remove convex brep
		MeshMod_MeshPolygonTagRemove(handle, MeshMod_PolygonConvexBRepTag);

	}
	MeshMod_MeshPolygonDataHasChanged(handle);
}
