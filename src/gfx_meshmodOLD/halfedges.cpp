#include "al2o3_platform/platform.h"
#include "render_meshmod/meshmod.h"
#include "halfedgedata/halfedgedata.h"
#include "polygons.h"
#include "halfedges.h"
#include "vertices.h"
#include "mesh.h"
#include <algorithm>
#include <string>
#include <unordered_set>

namespace MeshMod {

HalfEdges::HalfEdges(Mesh& owner_) :
	owner(owner_)
{
}

HalfEdges::~HalfEdges()
{
}

/**
Adds a half edge to the mesh.
Handles all the bookkeeping the half edge's correct, doesn't have a next edge index at this stage
@param svIndex start vertex index
@param evIndex end vertex index
@param poIndex polygon index the edge is attached to
@return index of half edge
*/
HalfEdgeIndex HalfEdges::add(VertexIndex const svIndex, VertexIndex const evIndex, PolygonIndex const faceIndex)
{
	// add half edge
	HalfEdgeData::HalfEdge e0;
	e0.startVertexIndex = svIndex;
	e0.endVertexIndex = evIndex;
	e0.polygonIndex = faceIndex;
	e0.next = InvalidHalfEdgeIndex;
	e0.prev = InvalidHalfEdgeIndex;

	auto& vertices = owner.getVertices();

	HalfEdgeIndex halfEdgeIndex = (HalfEdgeIndex) halfEdgesContainer.resizeForNewElement();
	halfEdge(halfEdgeIndex) = e0;

	auto& verticesHalfEdge = vertices.getOrAddAttribute<VertexData::HalfEdges>();

	// check for duplicates
	if(std::find(verticesHalfEdge[svIndex].halfEdgeIndexContainer.begin(),
				 verticesHalfEdge[svIndex].halfEdgeIndexContainer.end(),
				 halfEdgeIndex) == verticesHalfEdge[svIndex].halfEdgeIndexContainer.end())
	{
		verticesHalfEdge[svIndex].halfEdgeIndexContainer.push_back(halfEdgeIndex);
	}

	if(std::find(verticesHalfEdge[evIndex].halfEdgeIndexContainer.begin(),
				 verticesHalfEdge[evIndex].halfEdgeIndexContainer.end(),
				 halfEdgeIndex) == 
		verticesHalfEdge [evIndex].halfEdgeIndexContainer.end())
	{
		verticesHalfEdge[evIndex].halfEdgeIndexContainer.push_back(halfEdgeIndex);
	}

	return halfEdgeIndex;
}

void HalfEdges::remove(HalfEdgeIndex const index)
{
	if(!isValid(index)) return;

	Vertices& vertices = owner.getVertices();
	Polygons& polygons = owner.getPolygons();

	HalfEdgeData::HalfEdge& hedge = halfEdge(index);
	VertexData::HalfEdges& vertexHalfEdges = vertices.getAttribute<VertexData::HalfEdges>();

	// change the polygons any half edge to next if possible
	if (hedge.polygonIndex != InvalidPolygonIndex)
	{
		auto& polygon = polygons.polygon(hedge.polygonIndex);
		if (hedge.next == InvalidHalfEdgeIndex || hedge.next == index)
		{
			polygon.anyHalfEdge = InvalidHalfEdgeIndex;
			polygons.remove(hedge.polygonIndex);
		}
		else
		{
			polygon.anyHalfEdge = hedge.next;
		}
	}

	// link previous halfedge to next half edge (and vice versa)
	if (hedge.prev != InvalidHalfEdgeIndex)
	{
		HalfEdgeData::HalfEdge& prevHE = halfEdge(hedge.prev);
		prevHE.endVertexIndex = hedge.startVertexIndex;
		prevHE.next = hedge.next;
	}
	if(hedge.next != InvalidHalfEdgeIndex)
	{
		HalfEdgeData::HalfEdge& nextHE = halfEdge(hedge.next);
		nextHE.startVertexIndex = hedge.endVertexIndex;
		nextHE.prev = hedge.prev;
	}

	// remove from pair's pair 
	if (hedge.pair != InvalidHalfEdgeIndex)
	{
		auto& pedge = halfEdge(hedge.pair);
		pedge.pair = InvalidHalfEdgeIndex;
	}

	// from this half edge from start and end vertex half edge lists
	if (vertices.isValid(hedge.startVertexIndex))
	{
		VertexData::HalfEdge& svhe = vertexHalfEdges[hedge.startVertexIndex];
		auto sit = std::find(svhe.halfEdgeIndexContainer.begin(),
			svhe.halfEdgeIndexContainer.end(), index);
		if (sit != svhe.halfEdgeIndexContainer.end())
		{
			svhe.halfEdgeIndexContainer.erase(sit);
		}
	}

	if (vertices.isValid(hedge.endVertexIndex))
	{
		auto& evhe = vertexHalfEdges[hedge.endVertexIndex];
		auto eit = std::find(evhe.halfEdgeIndexContainer.begin(),
			evhe.halfEdgeIndexContainer.end(), index);
		if (eit != evhe.halfEdgeIndexContainer.end()) evhe.halfEdgeIndexContainer.erase(eit);
	}

	hedge.prev = InvalidHalfEdgeIndex;
	hedge.next = InvalidHalfEdgeIndex;
	hedge.pair = InvalidHalfEdgeIndex;
	hedge.polygonIndex = InvalidPolygonIndex;
	halfEdgesContainer.setValid(index, false);

}
void HalfEdges::removeEdge(HalfEdgeIndex const index)
{
	HalfEdgeData::HalfEdge& hedge = halfEdge(index);
	// remove pair half edge 
	if (hedge.pair != InvalidHalfEdgeIndex)
	{
		remove(hedge.pair);
	}
	remove(index);
}
void HalfEdges::repack()
{
	// repack all valid half edges
	auto& vertices = owner.getVertices();
	auto& halfEdges = owner.getHalfEdges();
	auto& polygons = owner.getPolygons();

	// [index] = existing half edge index of valid indices
	std::vector<HalfEdgeIndex> newToOld;
	std::vector<HalfEdgeIndex> oldToNew; // InvalidHalfEdgeIndex if no new index
	newToOld.reserve(halfEdgesContainer.size());
	oldToNew.reserve(halfEdgesContainer.size());

	for (auto i = 0u; i < halfEdgesContainer.size(); ++i)
	{
		HalfEdgeIndex hei = HalfEdgeIndex{ i };
		bool valid = isValid(hei);
		if (valid)
		{
			auto& halfEdge = halfEdges.halfEdge(hei);
			if (!isValid(halfEdge.prev) || !isValid(halfEdge.next))
			{
				valid = false;
			}
		}

		if(valid)
		{
			oldToNew.push_back(HalfEdgeIndex(newToOld.size()));
			newToOld.push_back(hei);
		}
		else
		{
			oldToNew.push_back(InvalidHalfEdgeIndex);
		}
	}

	// nothing to repack
	if (newToOld.size() == 0) return;
	
	std::unordered_set<PolygonIndex> toRemove;

	polygons.visitValid(
		[this, &polygons, &halfEdges, &oldToNew, &toRemove](PolygonIndex polyIndex) {
		auto& poly = polygons.polygon(polyIndex);

		if (poly.anyHalfEdge == InvalidHalfEdgeIndex)
		{
			toRemove.insert(polyIndex);
			return;
		}

		polygons.visitHalfEdges(polyIndex, 
			[this, &polygons, &halfEdges, &oldToNew, polyIndex, &toRemove](HalfEdgeIndex hei)
		{
			if (!isValid(hei))
			{
				toRemove.insert(polyIndex);
				return;
			}
		});
	});

	for (auto pindex : toRemove)
	{
		polygons.remove(pindex);
	}

	HalfEdgeElementsContainer newHalfEdgeCon;
	halfEdgesContainer.cloneTo(newHalfEdgeCon);
	newHalfEdgeCon.resize(newToOld.size());
	newHalfEdgeCon.resetValidFlags();

	for (size_t i = 0; i < newHalfEdgeCon.getSizeOfElementContainer(); ++i)
	{
		auto oldCon = halfEdgesContainer.getElementContainer(i);
		auto newCon = newHalfEdgeCon.getElementContainer(i);

		for (size_t heIndex = 0; heIndex < newToOld.size(); ++heIndex)
		{
			oldCon->unsafeCopyElementTo(*newCon, 
				newToOld[heIndex], 
				HalfEdgeIndex(heIndex));
		}
	}

	// half edge now remapped
	newHalfEdgeCon.cloneTo(halfEdgesContainer);

	auto& vertexHalfEdges = vertices.getAttribute<VertexData::HalfEdges>();
	for (auto newIndex = 0u; newIndex < getCount(); ++newIndex)
	{
		auto& halfEdge = halfEdges.halfEdge(HalfEdgeIndex(newIndex));

		halfEdge.prev = oldToNew[size_t(halfEdge.prev)];
		halfEdge.next = oldToNew[size_t(halfEdge.next)];
		auto oldIndex = newToOld[newIndex];

		if (halfEdge.pair != InvalidHalfEdgeIndex)
		{

			if(size_t(oldToNew[size_t(halfEdge.pair)]) >= newToOld.size())
				halfEdge.pair = InvalidHalfEdgeIndex;
			else
				halfEdge.pair = oldToNew[size_t(halfEdge.pair)];
		}
	}

	vertices.visitAll(
		[this, &halfEdges, &vertexHalfEdges, &oldToNew, &newToOld](VertexIndex vertexIndex)
	{
		auto& vheList = vertexHalfEdges[vertexIndex].halfEdgeIndexContainer;
		for (auto& heIndex : vheList)
		{
			auto remapHEIndex = oldToNew[size_t(heIndex)];
			if (size_t(remapHEIndex) >= newToOld.size())
				remapHEIndex = InvalidHalfEdgeIndex;

			heIndex = remapHEIndex;
		}
		vheList.erase(std::remove_if(vheList.begin(), vheList.end(), [](HalfEdgeIndex heIndex) {
			return heIndex == InvalidHalfEdgeIndex;
		}), vheList.end());
	});

	polygons.visitValid([this, &polygons, &oldToNew](PolygonIndex polyIndex) {
		auto& poly = polygons.polygon(polyIndex);
		assert(oldToNew[size_t(poly.anyHalfEdge)] != InvalidHalfEdgeIndex);
		poly.anyHalfEdge = oldToNew[size_t(poly.anyHalfEdge)];
	});
}
/**
Breaks all edge pairs for the entire mesh.
Break each half edge from its other half
*/
void HalfEdges::breakPairs()
{
	std::vector<HalfEdgeData::HalfEdge>::iterator edgeIt = halfEdges().begin();
	while(edgeIt != halfEdges().end())
	{
		(*edgeIt).pair = InvalidHalfEdgeIndex;
		++edgeIt;
	}
}

/**
Connects Half edge to the other half edges for the entire mesh.
Builds the half edge to half edge data structures
*/
void HalfEdges::connectPairs()
{
	auto const& vertices = owner.getVertices();
	auto const& polygons = owner.getPolygons();

	for(auto& poly : polygons.polygons())
	{
		if(!polygons.isValid(poly)) continue;

		const HalfEdgeIndex firstEdge = poly.anyHalfEdge;
		HalfEdgeIndex halfEdgeIndex = firstEdge;

		do
		{
			if (!isValid(halfEdgeIndex)) break;

			// scan around edges attached to the start vertex of this edge
			// pair up any edge with the same start and end
			HalfEdgeData::HalfEdge& he = halfEdge(halfEdgeIndex);
			VertexIndex startVert = he.startVertexIndex;
			VertexIndex endVert = he.endVertexIndex;
			if (startVert == InvalidVertexIndex || 
				endVert == InvalidVertexIndex)
				break;

			// swap indices for consistent ordering
			if(startVert > endVert)
			{
				std::swap(startVert, endVert);
			}
			auto const& vertexHalfEdges = vertices.getAttribute<VertexData::HalfEdges>();

			//iterate through the edges connect to the start vertex
			for(auto vheIndex : vertexHalfEdges.at(startVert).halfEdgeIndexContainer)
			{
				// check that we are not working on our own edge
				if(vheIndex != halfEdgeIndex)
				{
					// does this edge have the same start and end vertex indices, if so its a pair
					HalfEdgeData::HalfEdge& e0 = halfEdge(vheIndex);
					HalfEdgeData::HalfEdge& e1 = halfEdge(halfEdgeIndex);
					if (e0.pair == InvalidHalfEdgeIndex && e1.pair == InvalidHalfEdgeIndex)
					{
						if (((e0.startVertexIndex == startVert) &&
							(e0.endVertexIndex == endVert)) ||
							((e0.startVertexIndex == endVert) &&
							(e0.endVertexIndex == startVert)))
						{
							e0.pair = halfEdgeIndex;
							e1.pair = vheIndex;
						}
					}
				}
			}
			halfEdgeIndex = he.next;
		} while(halfEdgeIndex != firstEdge);
	}
}

void HalfEdges::visitAll(std::function<void(HalfEdgeIndex const)> const& func)
{
	for (auto i = 0u; i < halfEdgesContainer.size(); ++i)
	{
		func(HalfEdgeIndex(i));
	}
}

void HalfEdges::visitValid(std::function<void(HalfEdgeIndex const)> const& func)
{
	for (auto i = 0u; i < halfEdgesContainer.size(); ++i)
	{
		HalfEdgeIndex hei{ i };
		if (isValid(hei))
		{
			func(hei);
		}
	}
}

void HalfEdges::visitLoop(HalfEdgeIndex const firstHalfEdgeIndex, std::function<void(HalfEdgeIndex const)> const& func)
{
	HalfEdgeIndex halfEdgeIndex = firstHalfEdgeIndex;

	if (halfEdgeIndex == InvalidHalfEdgeIndex) return;

	// vists half edges for polygons
	do
	{
		if (!isValid(halfEdgeIndex)) break;
		func(halfEdgeIndex);

		halfEdgeIndex = halfEdge(halfEdgeIndex).next;
	} while (halfEdgeIndex != firstHalfEdgeIndex);
}

}
