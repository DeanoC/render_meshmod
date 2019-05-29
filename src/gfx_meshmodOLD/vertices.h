//
// Created by Computer on 31/07/2018.
//

#ifndef MESH_VERTICES_H
#define MESH_VERTICES_H

#include "meshmod.h"
#include "vertexdata/vertexcontainers.h"
#include "vertexdata/positionvertex.h"
#include "vertexdata/pointrepvertex.h"
#include "halfedgedata/halfedgecontainers.h"
#include "polygonsdata/polygoncontainers.h"
#include <functional>

namespace MeshMod {
class Polygons;
class HalfEdges;
class Mesh;

class Vertices
{
public:
	friend class Mesh;
	Vertices(Mesh& owner_);
	~Vertices();

	VerticesElementsContainer& getVerticesContainer() { return verticesContainer; }
	VerticesElementsContainer const& getVerticesContainer() const { return verticesContainer; }

	size_t getCount() const { return getVerticesContainer().size(); }

	VertexData::Position const& position(VertexIndex const index) const;
	VertexData::Position& position(VertexIndex const index);

	VertexData::Positions const& positions() const;
	VertexData::Positions& positions();

	template<typename attribute> bool hasAttribute() const;

	template<typename attribute> attribute const& getAttribute() const;
	template<typename attribute> attribute& getAttribute();
	template<typename attribute> attribute& getOrAddAttribute();
	template<typename attribute> attribute const& getAttribute(std::string const& subname) const;
	template<typename attribute> attribute& getOrAddAttribute(std::string const& subname);

	template<typename attribute> void removeAttributes();

	//! has the vertex been deleted, return false if deleted.
	bool isValid(VertexIndex index) const { return getVerticesContainer().isValid(index); }
	bool isValid(VertexData::Positions::const_iterator polyIt) const;
	bool isValid(VertexData::Position const& pos) const;

	//! aka degree the number of edges leaving a vertex
	unsigned int getValency(VertexIndex const vertexIndex) const;

	//! retrieves all the edges that connect to/from the specified vertex.
	void getVertexHalfEdges(VertexIndex const vertexIndex, HalfEdgeIndexContainer& halfEdgeList) const;
	//! get all vertices at same position as vertNum (using pointrep for speed).
	void getSimilarVertexIndices(VertexIndex const vertexIndex, VertexIndexContainer& vertList) const;
	//! are the two position indices at the same position (are 'similar').
	bool similarPosition(VertexIndex const i0, VertexIndex const i1) const;
	//! scan the vertex index list to see if have a samePosition return the index if we have else MM_INVALID_INDEX
	VertexIndex hasPosition(VertexIndex const i0, VertexIndexContainer const& vertexList) const;

	void visitAll(std::function<void(VertexIndex const)> const& func);
	void visitAll(std::function<void(VertexIndex const)> const& func) const;
	void visitValid(std::function<void(VertexIndex const)> const& func);
	void visitValid(std::function<void(VertexIndex const)> const& func) const;

	void visit(VertexIndexContainer const& indexList, std::function<void(VertexIndex const)> const& func);
	void visit(VertexIndexContainer const& indexList, std::function<void(VertexIndex const)> const& func) const;

	void visitHalfEdges(VertexIndex const vertexIndex, std::function<void(HalfEdgeIndex const)> const& func);
	void visitSimilarVertexIndices(VertexIndex const vertexIndex, std::function<void(VertexIndex const)> const& func);

	//! clones a vertex and returns the clones vertex index.
	VertexIndex clone(VertexIndex vertexToCopy);
	//! adds a position (does not check for duplicates), returns the index.
	VertexIndex add( const float x, const float y, const float z );
	//! removes the indicated vertex (position + extra data at index ).
	void remove(VertexIndex index);

	// collapses all similar vertices to this one
	void weldSimilar(VertexIndex vertexIndex_);

	void createPointReps(VertexData::Axis axis = VertexData::Axis::X, float fEpsilon = 1e-5f);

	void removeAllSimilarPositions(VertexData::Axis axis = VertexData::Axis::X, float fEpsilon = 1e-5f);

protected:
	// MM_INVALID_INDEX if no new index
	void repack(std::vector<VertexIndex>& oldToNewRemap);

	Mesh& owner;
	//! vertex element container.
	VerticesElementsContainer verticesContainer;

};

inline VertexData::Position const& Vertices::position(VertexIndex const index) const
{
	auto const& poss = positions();
	return poss[index];
}
inline VertexData::Position& Vertices::position(VertexIndex const index)
{
	auto& poss = positions();
	return poss.at(index);
}

inline VertexData::Positions const& Vertices::positions() const
{
	return *verticesContainer.getElement<VertexData::Positions>();
}

inline VertexData::Positions& Vertices::positions()
{
	return *verticesContainer.getElement<VertexData::Positions>();
}

template<typename attribute>
inline bool Vertices::hasAttribute() const
{
	return verticesContainer.getElement<attribute>() != nullptr;
}

template<typename attribute>
inline attribute const& Vertices::getAttribute() const
{
	return *verticesContainer.getElement<attribute>();
}
template<typename attribute>
inline attribute& Vertices::getAttribute()
{
	return *verticesContainer.getElement<attribute>();
}

template<typename attribute>
inline attribute& Vertices::getOrAddAttribute()
{
	return *verticesContainer.getOrAddElement<attribute>();
}

template<typename attribute>
inline attribute const& Vertices::getAttribute(std::string const& subname) const
{
	return *verticesContainer.getElement<attribute>(subname);
}
template<typename attribute>
inline attribute& Vertices::getOrAddAttribute(std::string const& subname)
{
	return *verticesContainer.getOrAddElement<attribute>(subname);
}

template<typename attribute>
inline void Vertices::removeAttributes()
{
	verticesContainer.removeElements<attribute>();
}


inline bool Vertices::isValid(VertexData::Positions::const_iterator polyIt) const
{
	VertexIndex index = positions().distance(polyIt);
	return verticesContainer.isValid(index);
}

inline bool Vertices::isValid(VertexData::Position const& pos) const
{
	VertexData::Position const* beginPtr = &(*positions().cbegin());

	VertexIndex index = (VertexIndex)(&pos - beginPtr);
	return verticesContainer.isValid(index);
}

}

#endif //MESH_VERTICES_H
