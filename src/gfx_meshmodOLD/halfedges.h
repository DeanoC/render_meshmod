//
// Created by Computer on 31/07/2018.
//

#ifndef MESH_HALFEDGES_H
#define MESH_HALFEDGES_H

#include "meshmod/halfedgedata/halfedgecontainers.h"
#include <functional>

namespace MeshMod {

class Vertices;
class Polygons;
class Mesh;

class HalfEdges
{
public:
	friend class Mesh;
	HalfEdges(Mesh& owner_);
	~HalfEdges();

	HalfEdgeElementsContainer& getHalfEdgesContainer() { return halfEdgesContainer; }
	HalfEdgeElementsContainer const& getHalfEdgesContainer() const { return halfEdgesContainer; }

	size_t getCount() const { return halfEdgesContainer.size(); }

	HalfEdgeData::HalfEdges& halfEdges();
	HalfEdgeData::HalfEdges const& halfEdges() const;

	HalfEdgeData::HalfEdge const& halfEdge(HalfEdgeIndex const index) const;
	HalfEdgeData::HalfEdge& halfEdge(HalfEdgeIndex const index);

	template<typename attribute> attribute const& getAttributes() const;
	template<typename attribute> attribute& getAttributes();
	template<typename attribute> attribute& getOrAddAttributes();
	template<typename attribute> attribute const& getAttributes(std::string const& subname) const;
	template<typename attribute> attribute& getOrAddAttributes(std::string const& subname);

	//! has the half edges been deleted, return false if deleted.
	bool isValid(HalfEdgeIndex const index) const { return halfEdgesContainer.isValid(index); }

	void visitAll(std::function<void(HalfEdgeIndex const)> const& func);
	void visitValid(std::function<void(HalfEdgeIndex const)> const& func);

	void visitLoop(HalfEdgeIndex const heIndex, std::function<void(HalfEdgeIndex const)> const& func);

	HalfEdgeIndex add(VertexIndex const svIndex, VertexIndex const evIndex, PolygonIndex const faceIndex);

	void remove(HalfEdgeIndex const index);
	void removeEdge(HalfEdgeIndex const index);

	//! breaks any paired half edges among on all polygons.
	void breakPairs();

	//! finds and hooks up any unpaired edges that should be paired.
	void connectPairs();

protected:
	void repack();

	Mesh& owner;
	//! half edge elements container.
	HalfEdgeElementsContainer halfEdgesContainer;
};

inline HalfEdgeData::HalfEdge const& HalfEdges::halfEdge(HalfEdgeIndex const index) const
{
	auto const& halfEdgesElements = halfEdges();
	return halfEdgesElements[index];
}
inline HalfEdgeData::HalfEdge& HalfEdges::halfEdge(HalfEdgeIndex const index)
{
	auto& halfEdgesElements = halfEdges();
	return halfEdgesElements[index];
}

inline HalfEdgeData::HalfEdges const& HalfEdges::halfEdges() const
{
	return *halfEdgesContainer.getElement<HalfEdgeData::HalfEdges>();
}

inline HalfEdgeData::HalfEdges& HalfEdges::halfEdges()
{
	return *halfEdgesContainer.getElement<HalfEdgeData::HalfEdges>();
}

template<typename attribute>
inline attribute const& HalfEdges::getAttributes() const
{
	return *halfEdgesContainer.getElement<attribute>();
}
template<typename attribute>
inline attribute& HalfEdges::getAttributes()
{
	return *halfEdgesContainer.getElement<attribute>();
}
template<typename attribute>
inline attribute& HalfEdges::getOrAddAttributes()
{
	return *halfEdgesContainer.getOrAddElement<attribute>();
}

template<typename attribute>
inline attribute const& HalfEdges::getAttributes(std::string const& subname) const
{
	return *halfEdgesContainer.getElement<attribute>(subname);
}
template<typename attribute>
inline attribute& HalfEdges::getOrAddAttributes(std::string const& subname)
{
	return *halfEdgesContainer.getOrAddElement<attribute>(subname);
}

}

#endif //MESH_HALFEDGES_H
