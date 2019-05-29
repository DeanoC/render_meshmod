/** \file MeshModMesh.cpp
   Geometry Optimiser Mesh implementation.
   (c) 2002 Deano Calver
 */

//---------------------------------------------------------------------------
// Local Defines
//---------------------------------------------------------------------------
#define DO_VALIDATION 0

#include "meshmod.h"
#include <algorithm>
#include <set>
#include <iterator>
#include "mesh.h"
#include "vertices.h"
#include "halfedges.h"
#include "polygons.h"
#include "meshmod/vertexdata/vertexcontainers.h"
#include "meshmod/vertexdata/positionvertex.h"
#include "meshmod/vertexdata/pointrepvertex.h"
#include "meshmod/halfedgedata/halfedgecontainers.h"

namespace
{
unsigned int dummy_UINTFloatMarker = 0xFFFFFFFF;
};

//---------------------------------------------------------------------------
// Global Variables
//---------------------------------------------------------------------------
namespace MeshMod {
const float s_epsilon1e_5 = 1e-5f;
const float s_floatMarker = *(reinterpret_cast<float *>( &dummy_UINTFloatMarker ));


/**
defualt ctor
*/
Mesh::Mesh(const std::string& name_, bool maintainPointReps_, bool maintainEdgeConnections_) :
		SceneObject("Mesh", name_),
		vertices(*new Vertices(*this)),
		halfEdges(*new HalfEdges(*this)),
		polygons(*new Polygons(*this))
{
	edits = (EditState)~0;
	maintain = 0;
	if(maintainPointReps_)
		maintain |= Maintenance::PointReps;
	if(maintainEdgeConnections_)
		maintain |= Maintenance::EdgeConnections;

	// helps the auto copier and diagnostics
	// default elements
	vertices.getVerticesContainer().addElements<VertexData::Positions>();
	vertices.getVerticesContainer().addElements<VertexData::HalfEdges>();
	vertices.getVerticesContainer().addElements<VertexData::PointReps>();

	polygons.getPolygonsContainer().addElements<PolygonData::Polygons>();
	halfEdges.getHalfEdgesContainer().addElements<HalfEdgeData::HalfEdges>();
}

// copy ctor
Mesh::Mesh(const Mesh& rhs) :
		SceneObject("Mesh", rhs.getName()),
		vertices(*new Vertices(*this)),
		halfEdges(*new HalfEdges(*this)),
		polygons(*new Polygons(*this))
{
	edits = rhs.edits;
	maintain = rhs.maintain;

	rhs.vertices.getVerticesContainer().cloneTo(vertices.getVerticesContainer());

	rhs.halfEdges.getHalfEdgesContainer().cloneTo(halfEdges.getHalfEdgesContainer());
	rhs.polygons.getPolygonsContainer().cloneTo(polygons.getPolygonsContainer());
	rhs.materialContainer.cloneTo(materialContainer);
}

Mesh& Mesh::operator=(Mesh const& rhs)
{
	name = rhs.getName();

	vertices.getVerticesContainer().clear();
	halfEdges.getHalfEdgesContainer().clear();
	polygons.getPolygonsContainer().clear();

	edits = rhs.edits;
	maintain = rhs.maintain;
	rhs.vertices.getVerticesContainer().cloneTo(vertices.getVerticesContainer());
	rhs.halfEdges.getHalfEdgesContainer().cloneTo(halfEdges.getHalfEdgesContainer());
	rhs.polygons.getPolygonsContainer().cloneTo(polygons.getPolygonsContainer());
	rhs.materialContainer.cloneTo(materialContainer);

	return (*this);

}


/**
default dtor
*/
Mesh::~Mesh()
{
}
void Mesh::updateEditState( MeshMod::Mesh::EditState editState_ )
{
	edits |= editState_;
}

void Mesh::updateFromEdits()
{
	if( edits & TopologyEdits)
	{
		vertices.getVerticesContainer().removeDerived( DerivedFromTopology );
		halfEdges.getHalfEdgesContainer().removeDerived( DerivedFromTopology );
		polygons.getPolygonsContainer().removeDerived( DerivedFromTopology );
		materialContainer.removeDerived( DerivedFromTopology );
		edits |= MaintenanceEdits;
	} else if(edits & TopologyAttributesEdits)
	{
		vertices.getVerticesContainer().removeDerived( DerivedFromAttributes );
		halfEdges.getHalfEdgesContainer().removeDerived( DerivedFromAttributes );
		polygons.getPolygonsContainer().removeDerived( DerivedFromAttributes );
		materialContainer.removeDerived( DerivedFromAttributes );
	} else if(edits & PositionEdits)
	{
		vertices.getVerticesContainer().removeDerived( DerivedFromPositions );
		halfEdges.getHalfEdgesContainer().removeDerived( DerivedFromPositions );
		polygons.getPolygonsContainer().removeDerived( DerivedFromPositions );
		materialContainer.removeDerived( DerivedFromPositions );
		edits |= MaintenanceEdits;
	} else if(edits & VertexAttributeEdits)
	{
		vertices.getVerticesContainer().removeDerived( DerivedFromAttributes );
		halfEdges.getHalfEdgesContainer().removeDerived( DerivedFromAttributes );
		polygons.getPolygonsContainer().removeDerived( DerivedFromAttributes );
		materialContainer.removeDerived( DerivedFromAttributes );
	}

	if (edits & MaintenanceEdits)
	{
		if (maintain & Maintenance::PointReps)
		{
			vertices.createPointReps();
			cleanAndRepack();
		}

		if (maintain & Maintenance::EdgeConnections)
		{
			halfEdges.breakPairs();
			halfEdges.connectPairs();
		}
	}

	validate();

	edits = NoEdits;
}

namespace {

//! links a float (X) to the vertex index
typedef std::pair<float, VertexIndex> sortItem;

//! sorts things based on the float value, you can then
//! get the original version via the index
struct PointRepSortCompare
{
	bool operator()(const sortItem& a, const sortItem& b)
	{
		return (a.first < b.first);
	}
};
}


/**
finds a material by name. currently linear search if that proves slow (i.e. lots of materials)
a map could easily be implemented.
*/
MaterialIndex Mesh::findMaterial(const char *pName) const
{
	const MaterialElementsContainer& matCon = getMaterialContainer();

	// no names, then must be invalid material
	auto nameEle = matCon.getElement<MeshData::MaterialNames>();
	if(nameEle) return InvalidMaterialIndex;

	// search for this name
	auto idenIt = nameEle->elements.cbegin();
	while(idenIt != nameEle->elements.cend())
	{
		if(idenIt->matName == pName)
		{
			return (MaterialIndex) (std::distance(nameEle->elements.cbegin(), idenIt));
		}
		++idenIt;
	}

	// can't find it
	return InvalidMaterialIndex;
}

Mesh *Mesh::rawClone() const
{
	return new Mesh(*this);
}

std::unique_ptr<Mesh> Mesh::clone() const
{
	return std::make_unique<Mesh>(*this);
}

void Mesh::validate() const
{
#if DO_VALIDATION
	// validate
	vertices.visitAll([this](VertexIndex vertexIndex) {
		assert(vertices.isValid(vertexIndex));

		std::vector<VertexIndex> similarVertices;
		similarVertices.push_back(vertexIndex);
		vertices.visitSimilarVertexIndices(vertexIndex, [this, &similarVertices](VertexIndex simVIndex) {
			assert(size_t(simVIndex) < vertices.getCount());
			assert(vertices.isValid(simVIndex));
			assert(std::find(similarVertices.cbegin(), similarVertices.cend(),simVIndex) == similarVertices.cend());
			similarVertices.push_back(simVIndex);
		});

		std::unordered_map<HalfEdgeIndex, int> visitedHalfEdges;
		vertices.visitHalfEdges(vertexIndex, [this, &visitedHalfEdges](HalfEdgeIndex heIndex) {
			assert(size_t(heIndex) < halfEdges.getCount());
			visitedHalfEdges[heIndex]++;
			assert(visitedHalfEdges[heIndex] <= 2);

			auto const& halfEdge = halfEdges.halfEdge(heIndex);

			assert(size_t(halfEdge.prev) < halfEdges.getCount());
			assert(size_t(halfEdge.next) < halfEdges.getCount());
			assert(size_t(halfEdge.startVertexIndex) < vertices.getCount());
			assert(size_t(halfEdge.endVertexIndex) < vertices.getCount());
			assert(size_t(halfEdge.polygonIndex) < polygons.getCount());
			if (halfEdge.pair != InvalidHalfEdgeIndex)
			{
				assert(size_t(halfEdge.pair) < halfEdges.getCount());
			}

			if (halfEdge.prev != halfEdge.next)
			{
				auto const& prevHalfEdge = halfEdges.halfEdge(halfEdge.prev);
				auto const& nextHalfEdge = halfEdges.halfEdge(halfEdge.next);
				assert(prevHalfEdge.next == heIndex);
				assert(nextHalfEdge.prev == heIndex);
				assert(prevHalfEdge.endVertexIndex == halfEdge.startVertexIndex);
				assert(nextHalfEdge.startVertexIndex == halfEdge.endVertexIndex);
			}

			assert(size_t(halfEdge.polygonIndex) < polygons.getCount());

			auto const& polygon = polygons.polygon(halfEdge.polygonIndex);
			assert(size_t(polygon.anyHalfEdge) < halfEdges.getCount());

		});
	});
	polygons.visitAll([this](PolygonIndex polygonIndex) {
		assert(polygons.isValid(polygonIndex));
		polygons.visitHalfEdges(polygonIndex, [this, polygonIndex](HalfEdgeIndex heIndex) {
			assert(halfEdges.isValid(heIndex));
			auto const& halfEdge = halfEdges.halfEdge(heIndex);
			assert(halfEdge.polygonIndex == polygonIndex);
			assert(size_t(halfEdge.startVertexIndex) < vertices.getCount());
			assert(size_t(halfEdge.endVertexIndex) < vertices.getCount());
		});
	});
#endif
}

void Mesh::cleanAndRepack()
{
	using namespace MeshMod;

	Vertices& vertices = getVertices();
	Polygons & polygons = getPolygons();

	// do we need to do any repacking
	bool anyInvalid = false;
	for (auto i = 0u; i < vertices.getCount(); ++i)
	{
		anyInvalid |= !vertices.isValid(VertexIndex(i));
	}
	for (auto i = 0u; i < polygons.getCount(); ++i)
	{
		anyInvalid |= !polygons.isValid(PolygonIndex(i));
	}

	if (!anyInvalid) return;

	// this does a complete rebuild, first we need to get all the valid indices
	std::vector<VertexIndexContainer> polys;
	polys.resize(polygons.getCount());

	for (auto i = 0u; i < polygons.getCount(); ++i)
	{
		polygons.getVertexIndices(PolygonIndex(i), polys[i]);
	}

	halfEdges.getHalfEdgesContainer().clear();
	polygons.getPolygonsContainer().clear();
	polygons.getPolygonsContainer().addElements<PolygonData::Polygons>();
	halfEdges.getHalfEdgesContainer().addElements<HalfEdgeData::HalfEdges>();

	vertices.getVerticesContainer().removeElements<VertexData::HalfEdges>();
	vertices.getVerticesContainer().addElements<VertexData::HalfEdges>();


	std::vector<VertexIndex> oldToNew;
	vertices.repack(oldToNew);

	VertexIndexContainer newPolyIndices;
	for (auto const& poly : polys)
	{
		if (poly.size() > 0)
		{
			newPolyIndices.resize(poly.size());
			for (auto i = 0u; i < poly.size(); ++i)
			{
				VertexIndex newIndex = oldToNew[size_t(poly[i])];
				assert(size_t(newIndex) < vertices.getCount());
				newPolyIndices[i] = newIndex;
			}
			polygons.addPolygon(newPolyIndices);
		}
	}

}

void Mesh::cleanAndRepackWIP()
{
	using namespace MeshMod;

	using VertexMarkers = VertexData::BoolMarkers<DerivedType::DerivedFromTopology>;
	using HalfEdgeMarkers = HalfEdgeData::BoolMarkers<DerivedType::DerivedFromTopology>;
	using PolygonMarkers = PolygonData::BoolMarkers<DerivedType::DerivedFromTopology>;

	Vertices& vertices = getVertices();
	HalfEdges& halfEdges = getHalfEdges();
	Polygons & polygons = getPolygons();

	auto& vertMarkers = vertices.getOrAddAttribute<VertexMarkers>();

	// go through all valid polygons, using valid halfEdges to mark valid vertices 
	polygons.visitValid([&vertices, &halfEdges, &polygons, &vertMarkers](PolygonIndex polygonIndex)
	{
		HalfEdgeIndexContainer polygonHalfEdgeIndices;
		polygons.getHalfEdgeIndices(polygonIndex, polygonHalfEdgeIndices);
		for (auto const& halfEdgeIndex : polygonHalfEdgeIndices)
		{
			auto const& halfEdge = halfEdges.halfEdge(halfEdgeIndex);
			if (vertices.isValid(halfEdge.startVertexIndex))
			{
				vertMarkers[halfEdge.startVertexIndex].marker = true;
			}
			if (vertices.isValid(halfEdge.endVertexIndex))
			{
				vertMarkers[halfEdge.endVertexIndex].marker = true;
			}
		}
	});
	// vertices have been marked and can be deleted and repacked
	for (auto& marker : vertMarkers)
	{
		if (marker.marker == false)
		{
			VertexIndex vi = vertMarkers.distance(marker);
			vertices.remove(vi);
		}
	}

	std::vector<VertexIndex> oldToNew;
	vertices.repack(oldToNew);

	auto& heMarkers = halfEdges.getOrAddAttributes<HalfEdgeMarkers>();
	// go through all valid polygons, using valid marking any which half edges are used and okay
	polygons.visitValid([&vertices, &halfEdges, &polygons, &heMarkers](PolygonIndex polygonIndex)
	{
		HalfEdgeIndexContainer polygonHalfEdgeIndices;
		polygons.getHalfEdgeIndices(polygonIndex, polygonHalfEdgeIndices);
		for (auto const& halfEdgeIndex : polygonHalfEdgeIndices)
		{
			heMarkers[halfEdgeIndex].marker = true;

			auto const& halfEdge = halfEdges.halfEdge(halfEdgeIndex);
			if (halfEdge.prev == InvalidHalfEdgeIndex)
			{
				heMarkers[halfEdgeIndex].marker = false;
			}
			else
			{
				auto const &prevhalfEdge = halfEdges.halfEdge(halfEdge.prev);
				assert(prevhalfEdge.next == halfEdgeIndex);
				if (prevhalfEdge.next != halfEdgeIndex)
					heMarkers[halfEdgeIndex].marker = false;
			}

			if (halfEdge.next == InvalidHalfEdgeIndex)
			{
				heMarkers[halfEdgeIndex].marker = false;
			}
			else
			{
				auto const &nexthalfEdge = halfEdges.halfEdge(halfEdge.next);
				assert(nexthalfEdge.prev == halfEdgeIndex);
				if (nexthalfEdge.prev != halfEdgeIndex)
					heMarkers[halfEdgeIndex].marker = false;
			}
		}
	});

	// now  remove marked half edges and repack
	for (auto& marker : heMarkers)
	{
		auto hei = heMarkers.distance(marker);
		HalfEdgeData::HalfEdge& halfEdge = halfEdges.halfEdge(hei);

		if (marker.marker == false)
		{
			halfEdges.remove(hei);
		}
	}
	halfEdges.repack();

	// finially mark valid polygons and kill the rest, repack at the end
	auto& polyMarkers = polygons.getOrAddAttribute<PolygonMarkers>();
	polygons.visitValid([&vertices, &halfEdges, &polygons, &polyMarkers](PolygonIndex polygonIndex) {
		auto& polyMarker = polyMarkers[polygonIndex];
		if (polygons.isValid(polygonIndex))
		{
			polyMarker.marker = true;
			polygons.visitHalfEdges(polygonIndex, [&halfEdges, &polyMarker](HalfEdgeIndex const heIndex)
			{
				if (!halfEdges.isValid(heIndex))
				{
					polyMarker.marker = false;
				}
			});
		}
		else 
		{
			polyMarker.marker = false;
		}
	});

	for (auto& marker : polyMarkers)
	{
		if (marker.marker == false)
		{
			auto poi = polyMarkers.distance(marker);
			polygons.remove(poi);
		}
	}

	polygons.repack();
}
template<class T, bool interpolatable, DerivedType derived>
inline VertexIndex Mesh::addVertexAttributeToFace(const VertexIndex vPosIndex,
												  const PolygonIndex fIndex,
												  BaseElements<T, Vertex_, interpolatable, derived> &eleContainer,
												  const T &data)
{
	// first scan the face for the position we have an index for
	VertexIndexContainer faceVertices;
	polygons.getVertexIndices(fIndex, faceVertices);
	VertexIndex vfPosIndex = vertices.hasPosition(vPosIndex, faceVertices);

	// if this fires the position passed in doesn't belong to the face
	// passed it, which makes this whole thing useless...
	assert(vfPosIndex != InvalidVertexIndex);

	// does this face vertex already have the attribute data
	if(eleContainer[vfPosIndex].equal(data) == true)
	{
		// it does, so nothing to do just return the index
		return vfPosIndex;
	} else
	{
		// nope, if it has a valid index
		// clone the old vertex and insert the data
		// else just make it a valid index by sticking
		// that data in there
		if(eleContainer[vfPosIndex].isValid())
		{
			VertexIndex vIndex = vertices.clone(vfPosIndex);
			eleContainer[vIndex] = data;
			return vIndex;
		} else
		{
			eleContainer[vfPosIndex] = data;
			return vfPosIndex;
		}
	}
}
} // end namespace