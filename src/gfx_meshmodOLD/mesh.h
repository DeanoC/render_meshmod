#pragma once
/** \file MeshModMesh.h
   A MeshMod Mesh.
   Holds vertices, faces, materials
   (c) 2002 Dean Calver
 */

#if !defined( MESH_MOD_MESH_H_ )
#define MESH_MOD_MESH_H_

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "meshmod.h"
#include "indextypes.h"
#include "sceneobject.h"
#include "meshdata/materialcontainers.h"

//---------------------------------------------------------------------------
// Enums and typedefs
//---------------------------------------------------------------------------
namespace MeshMod {
class Vertices;
class HalfEdges;
class Polygons;

//---------------------------------------------------------------------------
// Classes and structures
//---------------------------------------------------------------------------

/**
Mesh presents a geometry mesh.
The Mesh is main object used by the object system, a mesh has a list
of vertices, edges, faces and materials that represent any 3D object.
Once inside the mesh, indices are used to reference the source item,
The source element arrays can be retrieved to get at the actual data.

A quick overview of terminology
Position -	Position is an attribute that every vertex MUST have,
			a number of vertices can share a 'similar' position, this
			means they are the same physical 3D space but with
			different attributes (UV, normals).
Vertex -	A vertex is an unique N-D space point, each vertex has a
			point rep list that tell you of 'similar' vertices (share
			the same position).
			Vertices's can have there own list of elements.
Edge -		A edge connects 2 vertices, at the moment it is always an
			edge of a polygon. A full edge is made of 2 half-edges,
			half edges are connected to one other half edge, each half
			edge is linked to a particular polygon.
			Edges can have there own list of elements.
Half Edge -	Half edges connect polygons together and allow fast
			topology ops, A Mesh only tracks half-edges and not edges.
			An half-edge by itself marks a not connected polygon.
			Currently support for more than 2 manifold surfaces is
			unsupported at best and probably breaks things a lot.
Polygon -	A Mesh supports polygons fully, but some of the algorithms
			require simplex convex polygons and triangles are your best
			bet for pulling out renderer data
Triangle -	A polygon with 3 faces, always linear, simple and hardware
			friendly.
Face -		Another name for polygon,
			Faces have the own list of elements
Material -	Possible to be renamed but essentially a list of elements
			that are generally indexed by face attributes. Currently
			used for materials but possible there are other useful
			types.
*/
class Mesh : public SceneObject
{
public:
	friend class Vertices;
	friend class HalfEdges;
	friend class Polygons;

	enum EditState
	{
		NoEdits = 0,
		VertexAttributeEdits = 0x1,		// vertex attributes (except positions) have changed
		PositionEdits = 0x2,			// vertex positions have changed
		TopologyAttributesEdits = 0x4,	// topology based attributes (not actual topology changed)
		TopologyEdits = 0x8,			// topology (polygons/half edges) changed
		MaintenanceEdits = 0x16,			// change to the Maintenance settings
	};

	//! ctor.
	Mesh(const std::string &name_, bool maintainPointReps_ = true, bool maintainEdgeConnections_ = true);
	// clone from another mesh
	Mesh(Mesh const& rhs);

	Mesh& operator=(Mesh const& rhs);

	//! dtor.
	~Mesh();

	Mesh *rawClone() const;

	std::unique_ptr<Mesh> clone() const;

	//-----------------------------------
	// Mesh creation/alteration functions
	//----------------------------------
	void updateEditState( EditState editState_ );
	uint32_t getEdited() const { return edits; }
	void updateFromEdits();

	auto isMaintainPointReps() -> bool { return maintain & Maintenance::PointReps; }
	auto isMaintainEdgeConnections() -> bool { return maintain & Maintenance::EdgeConnections; }
	void maintainPointReps(bool enable) { maintain &= ~Maintenance::PointReps; maintain |= enable ? Maintenance::PointReps : 0; edits |= MaintenanceEdits; }
	void maintainEdgeConnections(bool enable) { maintain &= ~Maintenance::EdgeConnections; maintain |= enable ? Maintenance::EdgeConnections : 0; edits |= MaintenanceEdits; };

	Vertices& getVertices() { return vertices; }
	Vertices const& getVertices() const { return vertices; }

	HalfEdges& getHalfEdges() { return halfEdges; }
	HalfEdges const& getHalfEdges() const { return halfEdges; }

	Polygons& getPolygons() { return polygons; }
	Polygons const& getPolygons() const { return polygons; }


	template<class T, bool interpolatable, DerivedType derived>
	VertexIndex addVertexAttributeToFace(const VertexIndex vIndex,
										 const PolygonIndex fIndex,
										 BaseElements<T, Vertex_, interpolatable, derived> &eleContainer,
										 const T &data);

	//! get read write access meshes material container
	MaterialElementsContainer &getMaterialContainer() { return materialContainer; };
	//! get read only access meshes material container
	const MaterialElementsContainer &getMaterialContainer() const { return materialContainer; };
	//! find a particular material by name (note currently linear search assumes few materials...)
	MaterialIndex findMaterial(const char *pName) const;

	void validate() const;

	// note this loses halfedge and polgon extra attributes. So beware!
	void cleanAndRepack();

protected:
	// 
	void cleanAndRepackWIP();

	uint32_t edits;
	enum Maintenance
	{
		PointReps 		= 0x1,
		EdgeConnections = 0x2,
	};
	uint32_t maintain;

	//! material elements container.
	MaterialElementsContainer materialContainer;

	Polygons& polygons;
	HalfEdges& halfEdges;
	Vertices& vertices;
};



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
