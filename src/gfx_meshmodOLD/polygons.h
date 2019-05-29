//
// Created by Computer on 31/07/2018.
//

#ifndef MESH_MOD_POLYGONS_H
#define MESH_MOD_POLYGONS_H

#include "indextypes.h"
#include "meshmod/polygonsdata/polygoncontainers.h"
#include <functional>
namespace MeshMod {
class HalfEdges;
class Vertices;
class Mesh;

class Polygons
{
public:
	friend class Mesh;

	Polygons(Mesh& owner_);
	~Polygons();

	PolygonElementsContainer& getPolygonsContainer() { return polygonsContainer; }
	PolygonElementsContainer const& getPolygonsContainer() const { return polygonsContainer; }

	size_t getCount() const { return polygonsContainer.size(); }

	PolygonData::Polygon const& polygon(PolygonIndex const index) const;
	PolygonData::Polygon& polygon(PolygonIndex const index);

	PolygonData::Polygons const& polygons() const;
	PolygonData::Polygons& polygons();

	template<typename attribute> bool hasAttribute() const;
	template<typename attribute> attribute const& getAttribute() const;
	template<typename attribute> attribute& getAttribute();
	template<typename attribute> attribute& getOrAddAttribute();
	template<typename attribute> attribute const& getAttribute(std::string const& subname) const;
	template<typename attribute> attribute& getAttribute(std::string const& subname);
	template<typename attribute> attribute& getOrAddAttribute(std::string const& subname);

	//! has the polygon been deleted, return false if deleted.
	bool isValid(PolygonIndex const index) const { return polygonsContainer.isValid(index); }
	bool isValid(PolygonData::Polygons::const_iterator polyIt) const;
	bool isValid(PolygonData::Polygon const& poly) const;

	void visitAll(std::function<void(PolygonIndex const)> const& func) const;
	void visitValid(std::function<void(PolygonIndex const)> const& func) const;
	void visitValidVertices(std::function<void(PolygonIndex const, VertexIndex const)> const& func) const;
	void visitHalfEdges(PolygonIndex const polygonIndex, std::function<void(HalfEdgeIndex const)> const& func) const;
	void visitVertices(PolygonIndex const polygonIndex, std::function<void(VertexIndex const)> const& func) const;

	//! retrieves all the half edges making up a face.
	void getHalfEdgeIndices(PolygonIndex const index, HalfEdgeIndexContainer& halfEdgeList) const;

	//! retrieves all the vertices making up a face.
	void getVertexIndices(PolygonIndex const index, VertexIndexContainer& vertexList) const;

	// get the vertex count for a particular polygon
	auto getVertexCount(PolygonIndex const index) const -> size_t;

	//! retrieves the indices of all polygons that share an edge with the input polygon.
	void getSurroundingPolygonIndices(PolygonIndex const index, PolygonIndexContainer &polygonIndexContainer) const;

	PolygonIndex addPolygon(VertexIndexContainer const &indices, VertexIndex const baseVertex = VertexIndex(0));
	void addTriangles(VertexIndexContainer const &indices, VertexIndex const baseVertex = VertexIndex(0));
	void remove(PolygonIndex const index);


protected:
	void repack();

	Mesh& owner;
	//! polygon elements container.
	PolygonElementsContainer polygonsContainer;
};
inline PolygonData::Polygon const& Polygons::polygon(PolygonIndex const index) const
{
	auto const& polys = polygons();
	return polys.at(index);
}
inline PolygonData::Polygon& Polygons::polygon(MeshMod::PolygonIndex const index)
{
	auto& polys = polygons();
	return polys.at(index);
}

inline PolygonData::Polygons const& Polygons::polygons() const
{
	return *polygonsContainer.getElement<PolygonData::Polygons>();
}

inline PolygonData::Polygons& Polygons::polygons()
{
	return *polygonsContainer.getElement<PolygonData::Polygons>();
}

template<typename attribute>
inline bool Polygons::hasAttribute() const
{
	return polygonsContainer.getElement<attribute>() != nullptr;
}
template<typename attribute>
inline attribute const& Polygons::getAttribute() const
{
	assert(polygonsContainer.getElement<attribute>() != nullptr);
	return *polygonsContainer.getElement<attribute>();
}
template<typename attribute>
inline attribute& Polygons::getAttribute()
{
	return *polygonsContainer.getElement<attribute>();
}
template<typename attribute>
inline attribute& Polygons::getOrAddAttribute()
{
	return *polygonsContainer.getOrAddElement<attribute>();
}

template<typename attribute>
inline attribute const& Polygons::getAttribute(std::string const& subname) const
{
	return *polygonsContainer.getElement<attribute>(subname);
}
template<typename attribute>
inline attribute& Polygons::getAttribute(std::string const& subname)
{
	return *polygonsContainer.getElement<attribute>(subname);
}
template<typename attribute>
inline attribute& Polygons::getOrAddAttribute(std::string const& subname)
{
	return *polygonsContainer.getOrAddElement<attribute>(subname);
}

inline bool Polygons::isValid(PolygonData::Polygons::const_iterator polyIt) const
{
	PolygonIndex index = polygons().distance(polyIt);
	return polygonsContainer.isValid(index);
}

inline bool Polygons::isValid(PolygonData::Polygon const& poly) const
{
	PolygonData::Polygon const* polyBeginPtr = &(*polygons().cbegin());

	PolygonIndex index = (PolygonIndex)(&poly - polyBeginPtr);
	return polygonsContainer.isValid(index);
}


}

#endif //MESH_MOD_POLYGONS_H
