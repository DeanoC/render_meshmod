#pragma once
/** \file MeshModHalfEdgeVertex.h
   A vertex holding a list of half edges that join to this vertex.
   (c) 2006 Dean Calver
 */

#if !defined( MESH_MOD_POINT_REP_VERTEX_H_ )
#define MESH_MOD_POINT_REP_VERTEX_H_

#include "meshmod/meshmod.h"
#include "meshmod/varielements.h"

namespace MeshMod { namespace VertexData {
struct PointRep
{
	VertexIndex next;

	PointRep() {}

	PointRep(const VertexIndex n_) :
			next(n_) {}

	static constexpr std::string_view const getName() {
		using namespace std::literals;
		return "PointRep"sv; };
};
//! index of next vertex sharing position but with other data ( cyclic list )
typedef BaseElements<VertexData::PointRep, Vertex_, false, DerivedType::DerivedFromPositions> PointReps;

} } // end namespace

#endif // MESH_MOD_POINT_REP_VERTEX_H_
