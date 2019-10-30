#include "render_meshmod/helpers.h"
#include "render_meshmod/data/aabb.h"
#include "render_meshmod/vertex/position.h"
#include "render_meshmod/vertex/similar.h"
#include "render_meshmod/vertex/vertex2edges.h"
#include "render_meshmod/edge/halfedge.h"
#include "render_meshmod/edge/similar.h"
#include "render_meshmod/polygon/point.h"
#include "render_meshmod/polygon/line.h"
#include "render_meshmod/polygon/tribrep.h"
#include "render_meshmod/polygon/quadbrep.h"
#include "render_meshmod/polygon/convexbrep.h"
#include "render_meshmod/polygon/planeequation.h"
#include "mesh.h"

AL2O3_EXTERN_C uint8_t MeshMod_HelperGetPolygonEdges(MeshMod_MeshHandle handle, MeshMod_PolygonHandle phandle, MeshMod_EdgeHandle* outEdges, uint8_t maxOutEdges) {
	if(!MeshMod_MeshHandleIsValid(handle)) {
		return 0;
	}

	if(!MeshMod_MeshPolygonIsValid(handle, phandle)) {
		return 0;
	}

	MeshMod_Mesh *mesh = MeshMod_MeshHandleToPtr(handle);

	// lets see what type of polygon brep (+ points and lines we have)
	bool const isPoint = MeshMod_MeshPolygonTagExists(handle, MeshMod_PolygonPointTag) &&
			(!MeshMod_RegistryIsDefaultData(mesh->registry, MeshMod_PolygonPointTag.tag, MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonPointTag, phandle)));
	bool const isLine =  MeshMod_MeshPolygonTagExists(handle, MeshMod_PolygonLineTag) &&
			(!MeshMod_RegistryIsDefaultData(mesh->registry, MeshMod_PolygonLineTag.tag, MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonLineTag, phandle)));
	bool const isTri =  MeshMod_MeshPolygonTagExists(handle, MeshMod_PolygonTriBRepTag) &&
			(!MeshMod_RegistryIsDefaultData(mesh->registry, MeshMod_PolygonTriBRepTag.tag, MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonTriBRepTag, phandle)));
	bool const isQuad =  MeshMod_MeshPolygonTagExists(handle, MeshMod_PolygonQuadBRepTag) &&
			(!MeshMod_RegistryIsDefaultData(mesh->registry, MeshMod_PolygonQuadBRepTag.tag, MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonQuadBRepTag, phandle)));
	bool const isConvex =  MeshMod_MeshPolygonTagExists(handle, MeshMod_PolygonConvexBRepTag) &&
			(!MeshMod_RegistryIsDefaultData(mesh->registry, MeshMod_PolygonConvexBRepTag.tag, MeshMod_MeshPolygonTagHandleToPtr(handle, MeshMod_PolygonConvexBRepTag, phandle)));

	if(isPoint) {
		return 0;
	}

	if(isLine && maxOutEdges >= 1) {
		MeshMod_PolygonLine const * line = MeshMod_MeshPolygonLineTagHandleToPtr(handle, phandle, 0);
		outEdges[0] = line->edge;
		return 1;
	}

	if(isTri && maxOutEdges >= 3) {
		MeshMod_PolygonTriBRep const * tri = MeshMod_MeshPolygonTriBRepTagHandleToPtr(handle, phandle, 0);
		outEdges[0] = tri->edge[0];
		outEdges[1] = tri->edge[1];
		outEdges[2] = tri->edge[2];
		return 3;
	}

	if(isQuad && maxOutEdges >= 4) {
		MeshMod_PolygonQuadBRep const * quad = MeshMod_MeshPolygonQuadBRepTagHandleToPtr(handle, phandle, 0);
		outEdges[0] = quad->edge[0];
		outEdges[1] = quad->edge[1];
		outEdges[2] = quad->edge[2];
		outEdges[3] = quad->edge[3];
		return 4;
	}
	if(isConvex) {
		MeshMod_PolygonConvexBRep const* convex = MeshMod_MeshPolygonConvexBRepTagHandleToPtr(handle, phandle, 0);
		if(maxOutEdges >= convex->numEdges ) {
			for(uint8_t i=0;i < convex->numEdges;++i) {
				outEdges[i] = convex->edge[i];
			}
			return convex->numEdges;
		}
	}

	return 0;
}

AL2O3_EXTERN_C uint8_t MeshMod_HelperGetSimilarVertexPosition(MeshMod_MeshHandle handle, MeshMod_VertexHandle vhandle, MeshMod_VertexHandle* outVertices, uint8_t maxOutVertices) {
	if(!MeshMod_MeshHandleIsValid(handle)) {
		return 0;
	}

	if(!MeshMod_MeshVertexTagExists(handle, MeshMod_AddUserDataToVertexTag(MeshMod_VertexSimilarTag, 'P'))) {
		LOGWARNING("Vertex similar data not available! test will always fail!");
		return 0;
	}

	uint8_t similarCount = 0;

	MeshMod_VertexSimilar const* similar = MeshMod_MeshVertexSimilarTagHandleToPtr(handle, vhandle, 'P');
	while(!Handle_HandleEqual64(vhandle.handle, similar->handle)) {
		outVertices[similarCount] = *similar;
		if(++similarCount >= maxOutVertices) {
			return similarCount;
		}
		similar = MeshMod_MeshVertexSimilarTagHandleToPtr(handle, *similar, 'P');
	}

	return similarCount;
}
AL2O3_EXTERN_C uint8_t MeshMod_HelperGetSimilarEdges(MeshMod_MeshHandle handle, MeshMod_EdgeHandle ehandle, MeshMod_EdgeHandle* outEdges, uint8_t maxOutEdges) {
	if(!MeshMod_MeshHandleIsValid(handle)) {
		return 0;
	}

	if(!MeshMod_MeshEdgeTagExists(handle, MeshMod_AddUserDataToEdgeTag(MeshMod_EdgeSimilarTag, 0))) {
		LOGWARNING("Edge similar data not available! test will always fail!");
		return 0;
	}

	uint8_t similarCount = 0;

	MeshMod_EdgeSimilar const* similar = MeshMod_MeshEdgeSimilarTagHandleToPtr(handle, ehandle, 0);
	while(!Handle_HandleEqual64(ehandle.handle, similar->handle)) {
		outEdges[similarCount] = *similar;
		if(++similarCount >= maxOutEdges) {
			return similarCount;
		}
		similar = MeshMod_MeshEdgeSimilarTagHandleToPtr(handle, *similar, 0);
	}

	return similarCount;
}

AL2O3_EXTERN_C bool MeshMod_HelperAreVertexPositionsSimilar(MeshMod_MeshHandle handle, MeshMod_VertexHandle a, MeshMod_VertexHandle b) {
	if(!MeshMod_MeshHandleIsValid(handle)) {
		return false;
	}

	if( Handle_HandleEqual64(a.handle, b.handle)) {
		return true;
	}
	if(!MeshMod_MeshVertexTagExists(handle, MeshMod_AddUserDataToVertexTag(MeshMod_VertexSimilarTag, 'P'))) {
		LOGWARNING("Vertex similar data not available! test will always fail!");
		return false;
	}

	MeshMod_VertexSimilar const* similar = MeshMod_MeshVertexSimilarTagHandleToPtr(handle, a, 'P');
	while(!Handle_HandleEqual64(a.handle, similar->handle)) {
		if(Handle_HandleEqual64(b.handle, similar->handle)) {
			return true;
		}
		similar = MeshMod_MeshVertexSimilarTagHandleToPtr(handle, *similar, 'P');
	}

	return false;
}

AL2O3_EXTERN_C bool MeshMod_HelperAreEdgesSimilar(MeshMod_MeshHandle handle, MeshMod_EdgeHandle a, MeshMod_EdgeHandle b) {
	if(!MeshMod_MeshHandleIsValid(handle)) {
		return false;
	}

	if( Handle_HandleEqual64(a.handle, b.handle)) {
		return true;
	}

	if(!MeshMod_MeshEdgeTagExists(handle, MeshMod_EdgeSimilarTag)) {
		LOGWARNING("edge similar data not available! test will always fail!");
		return false;
	}

	MeshMod_EdgeSimilar const* similar = MeshMod_MeshEdgeSimilarTagHandleToPtr(handle, a, 0);
	while(!Handle_HandleEqual64(a.handle, similar->handle)) {
		if(Handle_HandleEqual64(b.handle, similar->handle)) {
			return true;
		}
		similar = MeshMod_MeshEdgeSimilarTagHandleToPtr(handle, *similar, 0);
	}

	return false;

}