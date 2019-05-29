#pragma once
/** \file MeshModHalfEdge.h
   A half edge type.
   (c) 2002 Dean Calver
 */

#if !defined( MESH_MOD_HALFEDGE_H_ )
#define MESH_MOD_HALFEDGE_H_

#include "meshmod/meshmod.h"
#include "meshmod/varicontainer.h"
#include "meshmod/varielements.h"
#include "meshmod/genericcontainters.h"
#include "halfedgedata.h"

namespace MeshMod { namespace HalfEdgeData {

//! boolean marker
template<DerivedType derived> using BoolMarkers = MeshMod::BoolMarkers<HalfEdge_, derived>;

//! a single float (unspecifed use, upto you use subname to identify)
template<DerivedType derived> using Float1Tuples = MeshMod::Float1Tuples<HalfEdge_, derived>;

//! a single unsigned int (unspecifed use, upto you use subname to identify)
template<DerivedType derived> using UnsignedInt1Tuples = MeshMod::UnsignedInt1Tuples<HalfEdge_,derived>;

typedef BaseElements< HalfEdgeData::HalfEdge, HalfEdge_, false, DerivedType::NotDerived> HalfEdges;


}
typedef VariContainer <Elements<HalfEdge_>> HalfEdgeElementsContainer;
} // end namespace
//---------------------------------------------------------------------------
// Prototypes
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Externals
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// End Header file
//---------------------------------------------------------------------------
#endif
