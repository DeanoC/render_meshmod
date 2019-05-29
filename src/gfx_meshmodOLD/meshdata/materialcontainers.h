#pragma once
/** \file MeshModVertex.h
   A vertex system.
   Holds  a number of named vertex elements, 
   contains a list of similar vertices (same position, different elements (uv's, normals)
   contains a list of polyMeshModns each vertex is attached to
   (c) 2002 Dean Calver
 */

#if !defined( MESH_MOD_MESH_DATA_MATERIAL_H_ )
#define MESH_MOD_MESH_DATA_MATERIAL_H_

#include "meshmod/meshmod.h"
#include "meshmod/varicontainer.h"
#include "meshmod/varielements.h"
#include "materialdata.h"

namespace MeshMod { namespace MeshData {
typedef BaseElements<MaterialData::Name, Mesh_, false, DerivedType::NotDerived> MaterialNames;
typedef BaseElements<MaterialData::Shader, Mesh_, false, DerivedType::NotDerived> MaterialShaders;

// these 3 are the old system, these days use the parameter system
typedef BaseElements<MaterialData::LightParams, Mesh_, false, DerivedType::NotDerived> MaterialLightParams;
typedef BaseElements<MaterialData::BackFace, Mesh_, false, DerivedType::NotDerived> MaterialBackFaces;
typedef BaseElements<MaterialData::Texture, Mesh_, false, DerivedType::NotDerived> MaterialTextures;
}

//! material bits and peices (usually just name, shader and parameter now...) attached to a mesh
typedef VariContainer<Elements<Mesh_> > MaterialElementsContainer;

} // end namespace

#endif // end MESH_MOD_MESH_DATA_MATERIAL_H_
