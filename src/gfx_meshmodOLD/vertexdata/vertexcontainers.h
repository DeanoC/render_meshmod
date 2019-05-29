#pragma once
/** \file MeshModVertex.h
   A vertex system.
   Holds  a number of named vertex elements, 
   contains a list of similar vertices (same position, different elements (uv's, normals)
   contains a list of polyMeshModns each vertex is attached to
   (c) 2002 Dean Calver
 */

#if !defined( MESH_MOD_VERTEX_H_ )
#define MESH_MOD_VERTEX_H_

#include "meshmod/meshmod.h"
#include "meshmod/varicontainer.h"
#include "meshmod/varielements.h"
#include "meshmod/genericcontainters.h"
#include "vertexdata.h"

namespace MeshMod { namespace VertexData {

//! boolean marker
template<DerivedType derived> using BoolMarkers = MeshMod::BoolMarkers<Vertex_, derived>;

//! a single float (unspecifed use, upto you use subname to identify)
template<DerivedType derived> using Float1Tuples = MeshMod::Float1Tuples<Vertex_, derived>;

//! a single unsigned int (unspecifed use, upto you use subname to identify)
template<DerivedType derived> using UnsignedInt1Tuples = MeshMod::UnsignedInt1Tuples<Vertex_,derived>;

//! bone indices and weights attached to this vertex
typedef BaseElements<VertexData::BoneWeight, Vertex_, true, DerivedType::NotDerived> BoneWeights;

//! vertex displacement attached to this vertex
typedef BaseElements<VertexData::PositionDelta, Vertex_, true, DerivedType::NotDerived> PositionDeltas;

//! an index remapping item mapping sorted to unsorted index
typedef BaseElements<VertexData::SortIndex, Vertex_, false, DerivedType::NotDerived> SortIndices;

//! indices of half edges that starts with this vertex
typedef BaseElements<VertexData::HalfEdge, Vertex_, false, DerivedType::NotDerived> HalfEdges;

} // end VertexData

//! A container for vertex elements
typedef VariContainer<Elements<Vertex_> > VerticesElementsContainer;

} // end namespace

#endif
