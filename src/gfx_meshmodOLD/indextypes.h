#pragma once
/** \file MeshModIndexTypes.h
	MeshMod is largely index based, therefore these are the basic
	unit used by this library
   (c) 2006 Dean Calver
 */

#if !defined( MESH_MOD_INDEX_TYPES_H_ )
#define MESH_MOD_INDEX_TYPES_H_

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <vector>

//---------------------------------------------------------------------------
// Enums and typedefs
//---------------------------------------------------------------------------
namespace MeshMod
{

	//---------------------------------------------------------------------------
	// Classes and structures
	//---------------------------------------------------------------------------
	//! vertex index type.
	enum class VertexIndex : uint32_t;
	//! half edge index type.
	enum class HalfEdgeIndex : uint32_t;
	//! face index type
	enum class PolygonIndex : uint32_t;
	//! material index type
	enum class MaterialIndex : uint32_t;
	//! scene node index
	enum class SceneNodeIndex : uint32_t;

	//! a list of vertex indices
	typedef std::vector<VertexIndex> VertexIndexContainer;
	//! a list of half edge indices
	typedef std::vector<HalfEdgeIndex> HalfEdgeIndexContainer;
	//! a list of face indices
	typedef std::vector<PolygonIndex> PolygonIndexContainer;

	//! value used to indicate an index is not assigned/deleted/invalid etc.
	inline static VertexIndex const InvalidVertexIndex = VertexIndex(~0);
	inline static HalfEdgeIndex const InvalidHalfEdgeIndex = HalfEdgeIndex(~0);
	inline static PolygonIndex const InvalidPolygonIndex = PolygonIndex(~0);
	inline static MaterialIndex const InvalidMaterialIndex = MaterialIndex(~0);
	inline static SceneNodeIndex const InvalidSceneNodeIndex = SceneNodeIndex(~0);

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
