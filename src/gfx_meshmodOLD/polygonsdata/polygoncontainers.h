#pragma once
/** \file polygon.h
   (c) 2002 Dean Calver
 */

#if !defined( MESH_MOD_POLYGON_CONTAINERS_H_)
#define MESH_MOD_POLYGON_CONTAINERS_H_

#include "meshmod/meshmod.h"
#include "meshmod/varicontainer.h"
#include "meshmod/varielements.h"
#include "meshmod/genericcontainters.h"
#include "polygondata.h"

//---------------------------------------------------------------------------
// Enums and typedefs
//---------------------------------------------------------------------------
namespace MeshMod {

//---------------------------------------------------------------------------
// Classes and structures
//---------------------------------------------------------------------------
namespace PolygonData {

//! boolean marker
template<DerivedType derived> using BoolMarkers = MeshMod::BoolMarkers<Polygon_, derived>;

//! a single float (unspecifed use, upto you use subname to identify)
template<DerivedType derived> using Float1Tuples = MeshMod::Float1Tuples<Polygon_, derived>;

//! a single unsigned int (unspecifed use, upto you use subname to identify)
template<DerivedType derived> using UnsignedInt1Tuples = MeshMod::UnsignedInt1Tuples<Polygon_, derived>;

typedef BaseElements<PolygonData::Polygon, Polygon_, false, DerivedType::NotDerived> Polygons;
typedef BaseElements<PolygonData::PlaneEquation, Polygon_, false, DerivedType::DerivedFromTopology> PlaneEquations;
typedef BaseElements<PolygonData::SortIndex, Polygon_, false, DerivedType::NotDerived> SortIndices;
typedef BaseElements<PolygonData::Material, Polygon_, false, DerivedType::NotDerived> Materials;

}

typedef VariContainer<Elements<Polygon_> > PolygonElementsContainer;
} // end namespace
#endif
