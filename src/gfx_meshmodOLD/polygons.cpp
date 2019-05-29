//
// Created by Computer on 31/07/2018.
//

#include <algorithm>
#include "vertices.h"
#include "halfedges.h"
#include "polygons.h"
#include "mesh.h"

namespace MeshMod {

Polygons::Polygons(Mesh& owner_) :
	owner(owner_)
{
}


Polygons::~Polygons()
{
}

/**
Adds a polygon to the mesh.
Adds a poly, creates edge/half edge data structures
@param indices - vertex indices the polygon is composed off
@param baseVertex - all vertex indices are based off this number
@return index of new polyMeshModn
*/
PolygonIndex Polygons::addPolygon(VertexIndexContainer const &indices, VertexIndex const baseVertex)
{
	// should always be convex simple polygons!!
	auto const& vertices = owner.getVertices();
	auto& halfEdges = owner.getHalfEdges();

	PolygonData::Polygon t0;
	// index of this polygon first edge
	HalfEdgeIndex firstEdge = HalfEdgeIndex(halfEdges.getCount());
	// if the input face data has 0 vertices
	if(indices.size() == 0)
	{
		t0.anyHalfEdge = InvalidHalfEdgeIndex;
	} else
	{
		t0.anyHalfEdge = firstEdge;
	}
	PolygonIndex const faceIndex = (PolygonIndex const) polygonsContainer.resizeForNewElement();
	polygon(faceIndex) = t0;

	// insert polygon at the end of the polygons list
	for(size_t i = 0u; i < indices.size(); i++)
	{
		const VertexIndex svIndex = VertexIndex( size_t(indices.at(i)) + size_t(baseVertex) );
		const VertexIndex evIndex = VertexIndex( size_t(indices.at((i + 1) % indices.size())) + size_t(baseVertex));

		if (!vertices.isValid(svIndex) || !vertices.isValid(evIndex)) return InvalidPolygonIndex;

		HalfEdgeIndex halfEdgeIndex = halfEdges.add(svIndex, evIndex, faceIndex);
		HalfEdgeData::HalfEdge& halfEdge = halfEdges.halfEdge(halfEdgeIndex);

		if(i == 0) halfEdge.prev = (HalfEdgeIndex)(size_t(firstEdge) + indices.size() - 1);
		else halfEdge.prev = (HalfEdgeIndex)(size_t(firstEdge) + i - 1);

		halfEdge.next = (HalfEdgeIndex)(size_t(firstEdge) + ((i + 1) % indices.size()));
	}

	return faceIndex;
}

/**
Adds a triangle list to the mesh.
Adds a poly, creates edge/half edge data structures
@param indices - vertex indices the polygon is composed off
@param baseVertex - all vertex indices are based off this number
@return index of new polyMeshModn
*/
void Polygons::addTriangles(VertexIndexContainer const &indices, VertexIndex const baseVertex)
{
	for (auto i = 0u; i < indices.size(); i+=3)
	{
		VertexIndexContainer tri{
			indices[i + 0],
			indices[i + 1],
			indices[i + 2],
		};

		addPolygon(tri, baseVertex);
	}
}

void Polygons::remove(PolygonIndex const index)
{
	if(!polygonsContainer.isValid(index)) return;

	PolygonData::Polygon& poly = polygon(index);
	auto& halfEdges = owner.getHalfEdges();

	HalfEdgeIndexContainer halfEdgeList;
	getHalfEdgeIndices(index, halfEdgeList);
	for (auto heIndex : halfEdgeList)
	{
		halfEdges.remove(heIndex);
	}

	poly.anyHalfEdge = InvalidHalfEdgeIndex;
	polygonsContainer.setValid(index, false);
}

/**
Gets all the faces surrounding (sharing edges) to specified face.
Uses the half edge pointers pair edges to get all faces surrounding
@param polygonIndex - face/polygon to get vertices from
@param faceList - return list of face
*/
void Polygons::getSurroundingPolygonIndices(const PolygonIndex polygonIndex, PolygonIndexContainer &polygonList) const
{
	if(polygonIndex == InvalidPolygonIndex) return;

	auto& halfEdges = owner.getHalfEdges();
	HalfEdgeIndex const firstEdge = polygon(polygonIndex).anyHalfEdge;
	HalfEdgeIndex halfEdgeIndex = firstEdge;

	do {
		auto const& halfEdge = halfEdges.halfEdge(halfEdgeIndex);
		if (halfEdge.pair != InvalidHalfEdgeIndex)
		{
			auto const& halfEdgePair = halfEdges.halfEdge(halfEdge.pair);
			assert(polygonIndex != halfEdgePair.polygonIndex);
			polygonList.push_back(halfEdgePair.polygonIndex);
		}
		halfEdgeIndex = halfEdges.halfEdge(halfEdgeIndex).next;
	} while( halfEdgeIndex != firstEdge );
}

/**
Gets all the edges attached to the face.
Uses the half edge next pointers to get all edges
@param polygonIndex - face/polygon to get edges from
@param halfEdgeList - return list of edges
*/
void Polygons::getHalfEdgeIndices(PolygonIndex const polygonIndex, HalfEdgeIndexContainer& halfEdgeList) const
{
	if(polygonIndex == InvalidPolygonIndex) return;
	if (!isValid(polygonIndex)) return;

	auto const& polys = polygons();
	auto const& halfEdges = owner.getHalfEdges();
	auto const& hes = halfEdges.halfEdges();

	HalfEdgeIndex const firstHalfEdgeIndex = polys[polygonIndex].anyHalfEdge;
	HalfEdgeIndex halfEdgeIndex = firstHalfEdgeIndex;

	if(halfEdgeIndex == InvalidHalfEdgeIndex) return;

	// gather half edges for triangles
	do
	{
		if (!halfEdges.isValid(halfEdgeIndex)) break;

		halfEdgeList.push_back(halfEdgeIndex);
		halfEdgeIndex = hes[halfEdgeIndex].next;
	} while(halfEdgeIndex != firstHalfEdgeIndex);
}
void Polygons::visitHalfEdges(PolygonIndex const polygonIndex, std::function<void(HalfEdgeIndex const)> const& func) const
{
	if (polygonIndex == InvalidPolygonIndex) return;
	if (!isValid(polygonIndex)) return;

	auto const& polys = polygons();
	auto& halfEdges = owner.getHalfEdges();

	halfEdges.visitLoop(polys[polygonIndex].anyHalfEdge, func);
}

/**
Gets all the vertices attached to the face.
Uses the half edge pointers to get all vertices that make up this face
@param polygonIndex - face/polygon to get vertices from
@param vertexList - return list of vertex
*/
void Polygons::getVertexIndices(PolygonIndex const polygonIndex, VertexIndexContainer& vertexList) const
{
	if(polygonIndex == InvalidPolygonIndex) return;

	auto const& vertices = owner.getVertices();
	auto const& halfEdges = owner.getHalfEdges();
	auto const& polys = polygons();
	auto const& hes = halfEdges.halfEdges();

	HalfEdgeIndex const firstHalfEdgeIndex = polys[polygonIndex].anyHalfEdge;
	HalfEdgeIndex halfEdgeIndex = firstHalfEdgeIndex;
	if(halfEdgeIndex == InvalidHalfEdgeIndex) return;

	// gather vertices for polygons
	do
	{
		if (!halfEdges.isValid(halfEdgeIndex)) break;

		auto const& halfEdge = hes[halfEdgeIndex];
		if (vertices.isValid(halfEdge.startVertexIndex))
		{
			vertexList.push_back(halfEdge.startVertexIndex);
		}

		assert((halfEdgeIndex == firstHalfEdgeIndex) ||
			(halfEdgeIndex != halfEdge.next));

		halfEdgeIndex = halfEdge.next;
	} while(halfEdgeIndex != firstHalfEdgeIndex);
}

auto Polygons::getVertexCount(PolygonIndex const polygonIndex_) const -> size_t
{
	if(polygonIndex_ == InvalidPolygonIndex) return 0;

	auto const& vertices = owner.getVertices();
	auto const& halfEdges = owner.getHalfEdges();
	auto const& polys = polygons();
	auto const& hes = halfEdges.halfEdges();

	HalfEdgeIndex const firstHalfEdgeIndex = polys[polygonIndex_].anyHalfEdge;
	HalfEdgeIndex halfEdgeIndex = firstHalfEdgeIndex;
	if(halfEdgeIndex == InvalidHalfEdgeIndex) return 0;

	size_t vertexCount = 0;
	do
	{
		if(!halfEdges.isValid(halfEdgeIndex)) break;

		auto const& halfEdge = hes[halfEdgeIndex];
		if(vertices.isValid(halfEdge.startVertexIndex))
		{
			vertexCount++;
		}

		assert((halfEdgeIndex == firstHalfEdgeIndex) ||
			   (halfEdgeIndex != halfEdge.next));

		halfEdgeIndex = halfEdge.next;
	} while(halfEdgeIndex != firstHalfEdgeIndex);

	return vertexCount;
}


void Polygons::visitVertices(PolygonIndex const polygonIndex, std::function<void(VertexIndex const)> const& func) const
{
	VertexIndexContainer polyIndices;
	polyIndices.reserve(10);

	getVertexIndices(polygonIndex, polyIndices);
	for (auto vi : polyIndices)
	{
		func(vi);
	}
}
void Polygons::visitValidVertices(std::function<void(PolygonIndex const, VertexIndex const)> const& func) const
{
	VertexIndexContainer polyIndices;
	polyIndices.reserve(10);

	visitValid([this, &polyIndices, &func](PolygonIndex const polygonIndex) {
		polyIndices.clear();
		getVertexIndices(polygonIndex, polyIndices);
		for (auto vi : polyIndices)
		{
			func(polygonIndex, vi);
		}
	});
}


void Polygons::repack()
{
	// remove all invalid polygons using a polygon remap table
	// not very efficient as does a edge pair rebuild rather than remap that

	auto const& vertices = owner.getVertices();
	auto& halfEdges = owner.getHalfEdges();

	// [index] = existing polygon index of valid indices
	std::vector<PolygonIndex> newToOld;
	std::vector<PolygonIndex> oldToNew; // MM_INVALID_INDEX if no new index
	newToOld.resize(size_t(polygonsContainer.size()));
	oldToNew.resize(size_t(polygonsContainer.size()));

	size_t validCount = 0;
	size_t invalidCount = 0;
	for (auto i = 0u; i < polygonsContainer.size(); ++i)
	{
		PolygonIndex pi = PolygonIndex(i);
		if(isValid(pi))
		{
			auto& poly = polygon(pi);
			assert(poly.anyHalfEdge != InvalidHalfEdgeIndex);

			newToOld[validCount] = pi;
			oldToNew[i] = (PolygonIndex) validCount;
			validCount++;
		}
		else
		{
			oldToNew[i] = InvalidPolygonIndex;
			invalidCount++;
		}
	}

	// nothing to repack
	if (invalidCount == 0) return;

	newToOld.resize(validCount);

	PolygonElementsContainer newPolygonCon;
	polygonsContainer.cloneTo(newPolygonCon);
	newPolygonCon.resize(validCount);
	newPolygonCon.resetValidFlags();

	for(size_t i = 0; i < newPolygonCon.getSizeOfElementContainer(); ++i)
	{
		auto oldCon = polygonsContainer.getElementContainer(i);
		auto newCon = newPolygonCon.getElementContainer(i);

		for(size_t polyIndex = 0; polyIndex < validCount; ++polyIndex)
		{ 
			oldCon->unsafeCopyElementTo(*newCon, 
				newToOld[polyIndex], 
				PolygonIndex(polyIndex));
		}
	}
	// polygons now remapped
	newPolygonCon.cloneTo(polygonsContainer);

	// update each halfedge of every polygon
	for(size_t polyIndex = 0; polyIndex < validCount; ++polyIndex)
	{
		auto& poly = polygon((PolygonIndex) polyIndex);

		assert(poly.anyHalfEdge != InvalidHalfEdgeIndex);

		HalfEdgeIndex const firstHalfEdgeIndex = poly.anyHalfEdge;
		HalfEdgeIndex halfEdgeIndex = firstHalfEdgeIndex;

		do
		{
			if (!halfEdges.isValid(halfEdgeIndex)) break;

			HalfEdgeData::HalfEdge& halfEdge = halfEdges.halfEdge(halfEdgeIndex);
			if(halfEdge.polygonIndex != InvalidPolygonIndex)
			{
				halfEdge.polygonIndex = (PolygonIndex) polyIndex;
			}
			halfEdgeIndex = halfEdge.next;
		} while(halfEdgeIndex != firstHalfEdgeIndex);
	}

	// this could (and should be done my remapping)
	halfEdges.breakPairs();
	halfEdges.connectPairs();
}

void Polygons::visitAll(std::function<void(PolygonIndex const)> const& func) const
{
	for (auto i = 0u; i < polygonsContainer.size(); ++i)
	{
		func(PolygonIndex(i));
	}
}

void Polygons::visitValid(std::function<void(PolygonIndex const)> const& func) const
{
	for (auto i = 0u; i < polygonsContainer.size(); ++i)
	{
		PolygonIndex pi{ i };
		if (isValid(pi))
		{
			func(pi);
		}
	}
}

}