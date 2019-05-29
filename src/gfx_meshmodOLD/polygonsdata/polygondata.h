#pragma once
/** \file polygondata.h
   (c) 2002 Dean Calver
 */
#if !defined( MESH_MOD_POLYGONDATA_H_ )
#define MESH_MOD_POLYGONDATA_H_

#include "meshmod/meshmod.h"

namespace MeshMod { namespace PolygonData {

struct Polygon
{
	// one of the half edge (which one doesn't matter)
	HalfEdgeIndex anyHalfEdge = InvalidHalfEdgeIndex;

	static constexpr std::string_view const getName() {
		using namespace std::literals;
		return "Polygon"sv; };
};

struct PlaneEquation
{
	Math::Plane planeEq;

	static constexpr std::string_view const getName() {
		using namespace std::literals;
		return "PlaneEquation"sv; };
};

struct SortIndex
{
	PolygonIndex index;

	static constexpr std::string_view const getName() {
		using namespace std::literals;
		return "SortIndex"sv; };
};

struct Material {
	MaterialIndex surfaceIndex;

	static constexpr std::string_view const getName() {
		using namespace std::literals;
		return "Material"sv; };
};

} } // end namespace
#endif
