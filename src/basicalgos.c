#include "render_meshmod/basicalgos.h"
#include "render_meshmod/helpers.h"
#include "render_meshmod/data/aabb.h"
#include "render_meshmod/vertex/position.h"
#include "render_meshmod/vertex/similar.h"
#include "render_meshmod/vertex/vertex2edges.h"
#include "render_meshmod/edge/halfedge.h"
#include "render_meshmod/edge/similar.h"
#include "render_meshmod/edge/endvertex.h"
#include "render_meshmod/polygon/tribrep.h"
#include "render_meshmod/polygon/quadbrep.h"
#include "render_meshmod/polygon/convexbrep.h"
#include "render_meshmod/polygon/planeequation.h"
#include "mesh.h"

AL2O3_EXTERN_C MeshMod_DataAabb3F const *MeshMod_MeshComputeExtents3F(MeshMod_MeshHandle handle,
																																			MeshMod_VertexTag tag) {

	MeshMod_MeshVertexTagEnsure(handle, tag);
	uint64_t hash = MeshMod_MeshVertexTagGetOrComputeHash(handle, tag);

	MeshMod_Tag dataTag = MeshMod_VertexTagToHashTag(tag) | 'A';
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
		MeshMod_VertexHandle vhandle = MeshMod_MeshVertexTagIterate(handle, tag, NULL);
		while (MeshMod_MeshVertexIsValid(handle, vhandle)) {
			Math_Vec3F d = *(Math_Vec3F *) MeshMod_MeshVertexTagHandleToPtr(handle, tag, vhandle);
			Math_ExpandAabb3F(&aabb->aabb, d);
			vhandle = MeshMod_MeshVertexTagIterate(handle, tag, &vhandle);
		}
	}

	return aabb;
}

AL2O3_EXTERN_C MeshMod_DataAabb2F const *MeshMod_MeshComputeExtents2F(MeshMod_MeshHandle handle,
																																			MeshMod_VertexTag tag) {

	MeshMod_MeshVertexTagEnsure(handle, tag);
	uint64_t hash = MeshMod_MeshVertexTagGetOrComputeHash(handle, tag);

	MeshMod_Tag dataTag = MeshMod_VertexTagToHashTag(tag) | 'A';
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
		MeshMod_VertexHandle vhandle = MeshMod_MeshVertexTagIterate(handle, tag, NULL);
		while (MeshMod_MeshVertexIsValid(handle, vhandle)) {
			Math_Vec2F d = *(Math_Vec2F *) MeshMod_MeshVertexTagHandleToPtr(handle, tag, vhandle);
			Math_ExpandAabb2F(&aabb->aabb, d);
			vhandle = MeshMod_MeshVertexTagIterate(handle, tag, &vhandle);
		}
	}

	return aabb;

}

// convert triangles or quad brep to the general convex type
AL2O3_EXTERN_C void MeshMod_MeshToConvexBRep(MeshMod_MeshHandle handle) {
	// if already valid convex data, nothing to do
	if (MeshMod_MeshPolygonTagExists(handle, MeshMod_PolygonConvexBRepTag)) {
		return;
	}

	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonConvexBRepTag);

	// convert triangles if the exist
	if (MeshMod_MeshPolygonTagExists(handle, MeshMod_PolygonTriBRepTag)) {
		MeshMod_PolygonHandle triHandle = MeshMod_MeshPolygonTagIterate(handle,
																																		MeshMod_PolygonTriBRepTag,
																																		NULL);
		while (MeshMod_MeshPolygonIsValid(handle, triHandle)) {
			MeshMod_PolygonTriBRep const *triData = MeshMod_MeshPolygonTriBRepTagHandleToPtr(handle, triHandle, 0);
			MeshMod_PolygonConvexBRep *convexData = MeshMod_MeshPolygonConvexBRepTagHandleToPtr(handle, triHandle, 0);

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
	if (MeshMod_MeshPolygonTagExists(handle, MeshMod_PolygonQuadBRepTag)) {
		MeshMod_PolygonHandle quadHandle = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonQuadBRepTag, NULL);
		while (MeshMod_MeshPolygonIsValid(handle, quadHandle)) {
			MeshMod_PolygonQuadBRep const *quadData = MeshMod_MeshPolygonQuadBRepTagHandleToPtr(handle, quadHandle, 0);
			MeshMod_PolygonConvexBRep *convexData = MeshMod_MeshPolygonConvexBRepTagHandleToPtr(handle, quadHandle, 0);

			convexData->numEdges = 4;
			convexData->edge[0] = quadData->edge[0];
			convexData->edge[1] = quadData->edge[1];
			convexData->edge[2] = quadData->edge[2];
			convexData->edge[3] = quadData->edge[3];
			quadHandle = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonQuadBRepTag, &quadHandle);
		};
		// remove quad brep
		MeshMod_MeshPolygonTagRemove(handle, MeshMod_PolygonQuadBRepTag);
	}

	MeshMod_MeshTopologyHasChanged(handle);
}

AL2O3_EXTERN_C void MeshMod_MeshTrianglate(MeshMod_MeshHandle handle) {

	MeshMod_MeshPolygonTagEnsure(handle, MeshMod_PolygonTriBRepTag);

	// convert quads if the exist
	if (MeshMod_MeshPolygonTagExists(handle, MeshMod_PolygonQuadBRepTag)) {
		MeshMod_PolygonHandle quadHandle = MeshMod_MeshPolygonTagIterate(handle,
																																		 MeshMod_PolygonQuadBRepTag,
																																		 NULL);

		while (MeshMod_MeshPolygonIsValid(handle, quadHandle)) {
			MeshMod_PolygonQuadBRep const *quadData = MeshMod_MeshPolygonQuadBRepTagHandleToPtr(handle, quadHandle, 0);
			MeshMod_PolygonTriBRep *triData0 = MeshMod_MeshPolygonTriBRepTagHandleToPtr(handle, quadHandle, 0);

			triData0->edge[0] = quadData->edge[0];
			triData0->edge[1] = quadData->edge[1];
			triData0->edge[2] = quadData->edge[2];
			MeshMod_PolygonHandle newTriHandle = MeshMod_MeshPolygonDuplicate(handle, quadHandle);
			MeshMod_MeshPolygonTagHandleToDefault(handle, MeshMod_PolygonQuadBRepTag, newTriHandle);
			MeshMod_EdgeHandle nedge2 = MeshMod_MeshEdgeDuplicate(handle, quadData->edge[2]);
			MeshMod_EdgeHandle nedge0 = MeshMod_MeshEdgeDuplicate(handle, quadData->edge[0]);

			MeshMod_PolygonTriBRep *triData1 = MeshMod_MeshPolygonTriBRepTagHandleToPtr(handle, newTriHandle, 0);
			triData1->edge[0] = nedge2;
			triData1->edge[1] = quadData->edge[3];
			triData1->edge[2] = nedge0;

			MeshMod_EdgeHalfEdge *hedge0 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, triData1->edge[0], 0);
			MeshMod_EdgeHalfEdge *hedge1 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, triData1->edge[1], 0);
			MeshMod_EdgeHalfEdge *hedge2 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, triData1->edge[2], 0);
			hedge0->polygon = newTriHandle;
			hedge1->polygon = newTriHandle;
			hedge2->polygon = newTriHandle;

			quadHandle = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonQuadBRepTag, &quadHandle);
		};

		// remove quad brep
		MeshMod_MeshPolygonTagRemove(handle, MeshMod_PolygonQuadBRepTag);
	}

	// convert convex polygon if the exist
	if (MeshMod_MeshPolygonTagExists(handle, MeshMod_PolygonConvexBRepTag)) {
		MeshMod_PolygonHandle convexHandle = MeshMod_MeshPolygonTagIterate(handle,
																																			 MeshMod_PolygonConvexBRepTag,
																																			 NULL);
		while (MeshMod_MeshPolygonIsValid(handle, convexHandle)) {
			// we just want the edge count before we invalidate the data so safest way
			uint8_t const numEdges = MeshMod_MeshPolygonConvexBRepTagHandleToPtr(handle, convexHandle, 0)->numEdges;

			// pre-allocate space for the new triangles (if any)
			MeshMod_PolygonHandle phandles[MeshMod_PolygonConvexMaxEdges - 2];
			phandles[0] = convexHandle; // reuse this handle for the first triangle
			for (uint8_t i = 1; i < numEdges - 2; i++) {
				phandles[i] = MeshMod_MeshPolygonDuplicate(handle, convexHandle);
				// invalidate the duplicated convex polygon data as this is going to be a triangle
				MeshMod_MeshPolygonTagHandleToDefault(handle, MeshMod_PolygonConvexBRepTag, phandles[i]);
			}
			// pointer won't change now
			MeshMod_PolygonConvexBRep *convexData = MeshMod_MeshPolygonConvexBRepTagHandleToPtr(handle, convexHandle, 0);

			// special case the first triangle as it replaces the polygon handle of the original convex
			MeshMod_PolygonTriBRep *triData0 = MeshMod_MeshPolygonTriBRepTagHandleToPtr(handle, convexHandle, 0);
			triData0->edge[0] = convexData->edge[0];
			triData0->edge[1] = convexData->edge[1];
			triData0->edge[2] = convexData->edge[2];

			for (uint8_t i = 2; i < convexData->numEdges - 1; i++) {
				MeshMod_PolygonTriBRep *triData1 = MeshMod_MeshPolygonTriBRepTagHandleToPtr(handle, phandles[i - 1], 0);
				triData1->edge[0] = MeshMod_MeshEdgeDuplicate(handle, convexData->edge[i]);
				triData1->edge[1] = convexData->edge[i + 1];
				triData1->edge[2] = MeshMod_MeshEdgeDuplicate(handle, convexData->edge[0]);

				MeshMod_EdgeHalfEdge *hedge0 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, triData1->edge[0], 0);
				MeshMod_EdgeHalfEdge *hedge1 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, triData1->edge[1], 0);
				MeshMod_EdgeHalfEdge *hedge2 = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, triData1->edge[2], 0);
				hedge0->polygon = phandles[i - 1];
				hedge1->polygon = phandles[i - 1];
				hedge2->polygon = phandles[i - 1];
			}

			convexHandle = MeshMod_MeshPolygonTagIterate(handle, MeshMod_PolygonConvexBRepTag, &convexHandle);
		}

		// remove convex brep
		MeshMod_MeshPolygonTagRemove(handle, MeshMod_PolygonConvexBRepTag);

	}
	MeshMod_MeshTopologyHasChanged(handle);
}

static MeshMod_MeshHandle s_mesh;
static MeshMod_VertexSortFunc s_vertexSort;
static MeshMod_EdgeSortFunc s_edgeSort;
static MeshMod_PolygonSortFunc s_polygonSort;

static int VertexHandleAdaptor(MeshMod_VertexHandle a, MeshMod_VertexHandle b) {
	return s_vertexSort(s_mesh, a, b);
}

static int EdgeHandleAdaptor(MeshMod_EdgeHandle a, MeshMod_EdgeHandle b) {
	return s_edgeSort(s_mesh, a, b);
}

static int PolygonHandleAdaptor(MeshMod_PolygonHandle a, MeshMod_PolygonHandle b) {
	return s_polygonSort(s_mesh, a, b);
}

#define SORT_NAME MeshMod_VertexHandleTagSort
#define SORT_TYPE MeshMod_VertexHandle
#define SORT_CMP(x, y) VertexHandleAdaptor(x, y)
#include "sort.h"

AL2O3_EXTERN_C CADT_VectorHandle MeshMod_MeshVertexTagSort(MeshMod_MeshHandle handle,
																													 MeshMod_VertexTag tag,
																													 MeshMod_VertexSortFunc sortFunc) {
	CADT_VectorHandle output = CADT_VectorCreate(sizeof(MeshMod_VertexHandle));
	if (!output) {
		return NULL;
	}
	ASSERT(!MeshMod_MeshHandleIsValid(s_mesh));
	s_mesh = handle;
	s_vertexSort = sortFunc;

	// gather all the valid handles for this tag
	MeshMod_VertexHandle viterator = MeshMod_MeshVertexTagIterate(handle, tag, NULL);
	while (MeshMod_MeshVertexIsValid(handle, viterator)) {
		CADT_VectorPushElement(output, &viterator);
		viterator = MeshMod_MeshVertexTagIterate(handle, tag, &viterator);
	}

	MeshMod_VertexHandleTagSort_quick_sort(CADT_VectorData(output), CADT_VectorSize(output));
	s_mesh.handle.handle = 0;
	return output;
}

#define SORT_NAME MeshMod_EdgeHandleTagSort
#define SORT_TYPE MeshMod_EdgeHandle
#define SORT_CMP(x, y) EdgeHandleAdaptor(x, y)
#include "sort.h"

AL2O3_EXTERN_C CADT_VectorHandle MeshMod_MeshEdgeTagSort(MeshMod_MeshHandle handle,
																												 MeshMod_EdgeTag tag,
																												 MeshMod_EdgeSortFunc sortFunc) {
	CADT_VectorHandle output = CADT_VectorCreate(sizeof(MeshMod_EdgeHandle));
	if (!output) {
		return NULL;
	}

	ASSERT(!MeshMod_MeshHandleIsValid(s_mesh));
	s_mesh = handle;
	s_edgeSort = sortFunc;

	// gather all the valid handles for this tag
	MeshMod_EdgeHandle eiterator = MeshMod_MeshEdgeTagIterate(handle, tag, NULL);
	while (MeshMod_MeshEdgeIsValid(handle, eiterator)) {
		CADT_VectorPushElement(output, &eiterator);
		eiterator = MeshMod_MeshEdgeTagIterate(handle, tag, &eiterator);
	}

	MeshMod_EdgeHandleTagSort_quick_sort(CADT_VectorData(output), CADT_VectorSize(output));
	s_mesh.handle.handle = 0;

	return output;
}

#define SORT_NAME MeshMod_PolygonHandleTagSort
#define SORT_TYPE MeshMod_PolygonHandle
#define SORT_CMP(x, y) PolygonHandleAdaptor(x, y)
#include "sort.h"

AL2O3_EXTERN_C CADT_VectorHandle MeshMod_MeshPolygonTagSort(MeshMod_MeshHandle handle,
																														MeshMod_PolygonTag tag,
																														MeshMod_PolygonSortFunc sortFunc) {
	CADT_VectorHandle output = CADT_VectorCreate(sizeof(MeshMod_PolygonHandle));
	if (!output) {
		return NULL;
	}
	ASSERT(!MeshMod_MeshHandleIsValid(s_mesh));
	s_mesh = handle;
	s_polygonSort = sortFunc;

	// gather all the valid handles for this tag
	MeshMod_PolygonHandle piterator = MeshMod_MeshPolygonTagIterate(handle, tag, NULL);
	while (MeshMod_MeshPolygonIsValid(handle, piterator)) {
		CADT_VectorPushElement(output, &piterator);
		piterator = MeshMod_MeshPolygonTagIterate(handle, tag, &piterator);
	}

	MeshMod_PolygonHandleTagSort_quick_sort(CADT_VectorData(output), CADT_VectorSize(output));
	s_mesh.handle.handle = 0;

	return output;
}

static int VertexPositionXYZSort(MeshMod_MeshHandle handle, MeshMod_VertexHandle ah, MeshMod_VertexHandle bh) {
	MeshMod_VertexPosition const *a = MeshMod_MeshVertexPositionTagHandleToPtr(handle, ah, 0);
	MeshMod_VertexPosition const *b = MeshMod_MeshVertexPositionTagHandleToPtr(handle, bh, 0);

	int const xd = (int const) ((b->x * 10000.0f) - (a->x * 10000.0f));
	int const yd = (int const) ((b->y * 10000.0f) - (a->y * 10000.0f));
	int const zd = (int const) ((b->z * 10000.0f) - (a->z * 10000.0f));

	if (xd == 0) {
		if (yd == 0) {
			return zd;
		}
		return yd;
	}
	return xd;
}

static int VertexPositionYXZSort(MeshMod_MeshHandle handle, MeshMod_VertexHandle ah, MeshMod_VertexHandle bh) {
	MeshMod_VertexPosition const *a = MeshMod_MeshVertexPositionTagHandleToPtr(handle, ah, 0);
	MeshMod_VertexPosition const *b = MeshMod_MeshVertexPositionTagHandleToPtr(handle, bh, 0);

	int const xd = (int const) ((b->x * 10000.0f) - (a->x * 10000.0f));
	int const yd = (int const) ((b->y * 10000.0f) - (a->y * 10000.0f));
	int const zd = (int const) ((b->z * 10000.0f) - (a->z * 10000.0f));

	if (yd == 0) {
		if (xd == 0) {
			return zd;
		}
		return xd;
	}
	return yd;
}

static int VertexPositionZXYSort(MeshMod_MeshHandle handle, MeshMod_VertexHandle ah, MeshMod_VertexHandle bh) {
	MeshMod_VertexPosition const *a = MeshMod_MeshVertexPositionTagHandleToPtr(handle, ah, 0);
	MeshMod_VertexPosition const *b = MeshMod_MeshVertexPositionTagHandleToPtr(handle, bh, 0);

	int const xd = (int const) ((b->x * 10000.0f) - (a->x * 10000.0f));
	int const yd = (int const) ((b->y * 10000.0f) - (a->y * 10000.0f));
	int const zd = (int const) ((b->z * 10000.0f) - (a->z * 10000.0f));

	if (zd == 0) {
		if (xd == 0) {
			return yd;
		}
		return xd;
	}
	return zd;
}

AL2O3_EXTERN_C void MeshMod_MeshComputeSimilarPositions(MeshMod_MeshHandle handle, float similarity) {

	// first work out the longest axis, we will use that to sort along
	MeshMod_DataAabb3F const *aabb = MeshMod_MeshComputeExtents3F(handle, MeshMod_VertexPositionTag);
	size_t maxIndex = Math_HorizontalMaxIndex3F(Math_SubVec3F(aabb->aabb.maxExtent, aabb->aabb.minExtent));
	CADT_VectorHandle sortedForEAndWhizz = NULL;
	if (maxIndex == 0) {
		sortedForEAndWhizz = MeshMod_MeshVertexTagSort(handle, MeshMod_VertexPositionTag, VertexPositionXYZSort);
	} else if (maxIndex == 1) {
		sortedForEAndWhizz = MeshMod_MeshVertexTagSort(handle, MeshMod_VertexPositionTag, VertexPositionYXZSort);
	} else {
		sortedForEAndWhizz = MeshMod_MeshVertexTagSort(handle, MeshMod_VertexPositionTag, VertexPositionZXYSort);
	}

	// remove existing similar position data and new
	MeshMod_VertexTag const tag = MeshMod_AddUserDataToVertexTag(MeshMod_VertexSimilarTag, 'P');
	MeshMod_MeshVertexTagRemove(handle, tag);
	MeshMod_MeshVertexTagEnsure(handle, tag);

	for (int i = 0; i < (int) CADT_VectorSize(sortedForEAndWhizz); ++i) {
		MeshMod_VertexHandle vh = *(MeshMod_VertexHandle *) CADT_VectorAt(sortedForEAndWhizz, i);
		MeshMod_VertexPosition const *start = MeshMod_MeshVertexPositionTagHandleToPtr(handle, vh, 0);
		MeshMod_VertexHandle *t = MeshMod_MeshVertexSimilarTagHandleToPtr(handle, vh, 'P');
		*t = vh; // start with self linked

		// we know the list is in sorted order so we search backwards marking any within a fudged similarity to our
		// similarity ring
		int curI = i - 1;
		while (curI >= 0) {
			MeshMod_VertexHandle ah = *(MeshMod_VertexHandle *) CADT_VectorAt(sortedForEAndWhizz, curI);
			MeshMod_VertexPosition const *a = MeshMod_MeshVertexPositionTagHandleToPtr(handle, ah, 0);

			// if we are this far away along the sorted axis we can't be similar
			Math_Vec3F const axialdistance = Math_SubVec3F(*a, *start);
			if (axialdistance.v[maxIndex] > similarity) {
				break;
			}

			// check if we are similar by seeing if withing a sphere of similarity distance radius
			float const distance = Math_LengthVec3F(axialdistance);
			if (distance < similarity) {
				// link current vertex into the ring
				MeshMod_VertexHandle *l = MeshMod_MeshVertexSimilarTagHandleToPtr(handle, ah, 'P');
				*t = *l;
				*l = vh;
				// no more iteration needed, as the vertex we just linked to has already search backwards and linked
				break;
			}
			curI--;
		}
	}

	CADT_VectorDestroy(sortedForEAndWhizz);
}

AL2O3_EXTERN_C void MeshMod_MeshGenerateEdgeEndVertex(MeshMod_MeshHandle handle) {
	if (MeshMod_MeshEdgeTagExists(handle, MeshMod_EdgeEndVertexTag)) {
		return;
	}

	MeshMod_MeshEdgeTagEnsure(handle, MeshMod_EdgeEndVertexTag);
	MeshMod_MeshEdgeTagSetTransitive(handle,MeshMod_EdgeEndVertexTag, true);

	MeshMod_PolygonHandle phandle = MeshMod_MeshPolygonIterate(handle, NULL);
	MeshMod_EdgeHandle ehandles[16];

	while (MeshMod_MeshPolygonIsValid(handle, phandle)) {
		uint8_t const numEdges = MeshMod_HelperGetPolygonEdges(handle, phandle, ehandles, 16);

		for (int i = 0; i < numEdges; ++i) {
			MeshMod_EdgeHandle nextEdge = ehandles[(i < numEdges - 1 ? i + 1 : 0)];
			MeshMod_EdgeHalfEdge const *ehe = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, nextEdge, 0);
			MeshMod_EdgeEndVertex *ev = MeshMod_MeshEdgeEndVertexTagHandleToPtr(handle, ehandles[i], 0);
			*ev = ehe->vertex;
		}

		phandle = MeshMod_MeshPolygonIterate(handle, &phandle);
	}
}

AL2O3_EXTERN_C void MeshMod_MeshComputeVertex2Edges(MeshMod_MeshHandle handle) {

	if (MeshMod_MeshVertexTagExists(handle, MeshMod_Vertex2EdgesTag)) {
		return;
	}

	MeshMod_MeshVertexTagEnsure(handle, MeshMod_Vertex2EdgesTag);
	MeshMod_MeshVertexTagSetTransitive(handle, MeshMod_Vertex2EdgesTag, true);

	MeshMod_PolygonHandle phandle = MeshMod_MeshPolygonIterate(handle, NULL);
	MeshMod_EdgeHandle ehandles[16];

	while (MeshMod_MeshPolygonIsValid(handle, phandle)) {
		uint8_t const numEdges = MeshMod_HelperGetPolygonEdges(handle, phandle, ehandles, 16);

		for (int i = 0; i < numEdges; ++i) {
			// both incoming and outgoing half edges are stored in the vertex 2 edge element
			MeshMod_EdgeHalfEdge const *ehe = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, ehandles[i], 0);
			MeshMod_Vertex2Edges *v2e = MeshMod_MeshVertex2EdgesTagHandleToPtr(handle, ehe->vertex, 0);
			MeshMod_EdgeHandle const edges[2] = {ehandles[(i > 0 ? i : numEdges)-1], ehandles[i]};

			ASSERT(MeshMod_MeshEdgeIsValid(handle, edges[0]));
			ASSERT(MeshMod_MeshEdgeIsValid(handle, edges[1]));

			for (int j = 0; j < 2; ++j) {
				bool alreadyIn = false;
				for (int k = 0; k < v2e->numEdges; ++k) {
					if (Handle_HandleEqual64(v2e->edges[k].handle, edges[j].handle)) {
						alreadyIn = true;
						break;
					}
				}
				if (!alreadyIn) {
					// vertex2edges have embedded space but can grew if needed, below handles the transistion
					if(v2e->numEdges == 0) {
						v2e->edges = v2e->embedded;
					} else if(v2e->numEdges+1 == MeshMod_Vertex2EdgesMaxEmbedded) {
						MeshMod_EdgeHandle * tmp = (MeshMod_EdgeHandle *) MEMORY_MALLOC(sizeof(Handle_Handle64) * (v2e->numEdges + 1));
						memcpy(tmp, v2e->embedded, sizeof(Handle_Handle64) * MeshMod_Vertex2EdgesMaxEmbedded);
						v2e->edges = tmp;
					} else if(v2e->numEdges+1 > MeshMod_Vertex2EdgesMaxEmbedded) {
						v2e->edges =
								(MeshMod_EdgeHandle *) MEMORY_REALLOC(v2e->edges, sizeof(Handle_Handle64) * (v2e->numEdges + 1));
					}

					v2e->edges[v2e->numEdges++] = edges[j];
				}
			}
		}

		phandle = MeshMod_MeshPolygonIterate(handle, &phandle);
	}
}

AL2O3_EXTERN_C void MeshMod_MeshComputeSimilarEdges(MeshMod_MeshHandle handle) {
	if (MeshMod_MeshEdgeTagExists(handle, MeshMod_EdgeSimilarTag)) {
		return;
	}

	MeshMod_MeshComputeSimilarPositions(handle, 1e-5f);
	MeshMod_MeshComputeVertex2Edges(handle);
	MeshMod_MeshGenerateEdgeEndVertex(handle);

	MeshMod_MeshEdgeTagEnsure(handle, MeshMod_EdgeSimilarTag);
	MeshMod_MeshEdgeTagSetTransitive(handle, MeshMod_EdgeSimilarTag, true);

	MeshMod_PolygonHandle phandle = MeshMod_MeshPolygonIterate(handle, NULL);
	MeshMod_EdgeHandle ehandles[16];

	while (MeshMod_MeshPolygonIsValid(handle, phandle)) {
		uint8_t const numEdges = MeshMod_HelperGetPolygonEdges(handle, phandle, ehandles, 16);

		for (int i = 0; i < numEdges; ++i) {
			MeshMod_EdgeHandle *ourLink = MeshMod_MeshEdgeSimilarTagHandleToPtr(handle, ehandles[i], 0);
			if(MeshMod_MeshEdgeIsValid(handle, *ourLink)) {
				// if our similar link is valid then we are already in the ring, so nothing to do
				continue;
			}
			*ourLink = ehandles[i];

			MeshMod_EdgeHalfEdge const *che = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, ehandles[i], 0);
			MeshMod_EdgeEndVertex const *cev = MeshMod_MeshEdgeEndVertexTagHandleToPtr(handle, ehandles[i], 0);
			MeshMod_VertexHandle curSV;
			MeshMod_VertexHandle curEV;
			// sort into a fixed order for easier comparision
			if(Handle_HandleDistance64( che->vertex.handle, cev->handle) > 0) {
				curSV = che->vertex;
				curEV = *cev;
			} else {
				curSV = *cev;
				curEV = che->vertex;
			}
			MeshMod_VertexHandle similarVH[16];
			uint8_t similarVHCountS = MeshMod_HelperGetSimilarVertexPosition(handle, curSV, similarVH, 16);

			for(uint8_t j=0;j < similarVHCountS;++j) {
				MeshMod_Vertex2Edges const *v2e = MeshMod_MeshVertex2EdgesTagHandleToPtr(handle, similarVH[j], 0);
				for (uint8_t k = 0; k < v2e->numEdges; ++k) {
					MeshMod_EdgeHalfEdge const *sehe = MeshMod_MeshEdgeHalfEdgeTagHandleToPtr(handle, v2e->edges[k], 0);
					MeshMod_EdgeEndVertex const *sev = MeshMod_MeshEdgeEndVertexTagHandleToPtr(handle, v2e->edges[k], 0);

					MeshMod_VertexHandle simSV;
					MeshMod_VertexHandle simEV;
					if(Handle_HandleDistance64(sehe->vertex.handle, sev->handle) > 0) {
						simSV = sehe->vertex;
						simEV = *sev;
					} else {
						simSV = *sev;
						simEV = sehe->vertex;
					}
					bool const isSimilarS = MeshMod_HelperAreVertexPositionsSimilar(handle, curSV, simSV);
					bool const isSimilarE = MeshMod_HelperAreVertexPositionsSimilar(handle, curEV, simEV);

					if(isSimilarS && isSimilarE) {
						// similar (both start and end match)
						ASSERT(MeshMod_MeshEdgeIsValid(handle, v2e->edges[k]));
						MeshMod_EdgeHandle *l = MeshMod_MeshEdgeSimilarTagHandleToPtr(handle, v2e->edges[k], 0);
						if(!MeshMod_MeshEdgeIsValid(handle, *l)) {
							*l = *ourLink;
							*ourLink = v2e->edges[k];
						} else {
							MeshMod_EdgeHandle tmp = *l;
							*l = ehandles[i];
							*ourLink = tmp;

						}
					}
				}
			}
		}

		phandle = MeshMod_MeshPolygonIterate(handle, &phandle);
	}
}

AL2O3_EXTERN_C void MeshMod_MeshComputePlaneEquations(MeshMod_MeshHandle handle) {
}
