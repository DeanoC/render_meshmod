#pragma once
/** \file halfedgedata.h
   A half edge type.
   (c) 2002 Dean Calver
 */

#if !defined( MESH_MOD_HALFEDGEDATA_H_ )
#define MESH_MOD_HALFEDGEDATA_H_

#include "meshmod/meshmod.h"
#include "meshmod/varicontainer.h"
#include "meshmod/varielements.h"

namespace MeshMod { namespace HalfEdgeData {
struct HalfEdge
{
	//! index of vertex that starts this half edge
	VertexIndex startVertexIndex = InvalidVertexIndex;
	//! index of vertex that ends this half edge
	VertexIndex endVertexIndex = InvalidVertexIndex;
	//! polygon this edge is connected to
	PolygonIndex polygonIndex = InvalidPolygonIndex;

	//! the other half edge this half edges is paired with
	HalfEdgeIndex pair = InvalidHalfEdgeIndex;

	//! previous half edge in polygon (counter clockwise)
	HalfEdgeIndex prev = InvalidHalfEdgeIndex;

	//! next half edge in polygon (clockwise)
	HalfEdgeIndex next = InvalidHalfEdgeIndex;

	static constexpr std::string_view const getName() {
		using namespace std::literals;
		return "HalfEdge"sv; };
};
}} // end namespace

#endif //MESH_HALFEDGEDATA_H
