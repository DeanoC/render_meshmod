#pragma once
#ifndef GFX_MESHMOD_MESHMOD_H
#define GFX_MESHMOD_MESHMOD_H

#include "al2o3_platform/platform.h"
#include "render_meshmod/handles.h"

// the top byte is the MeshMod_Type (ascii 'V', 'E', 'P')
// the next byte is sub type data (currently ascii 'H' for hash)
// the bottom byte of a tag are ignored by the registry and can be used to hold user data
typedef uint64_t MeshMod_Tag;

typedef enum MeshMod_Type {
	MeshMod_TypeVertex = 'V',
	MeshMod_TypeEdge = 'E',
	MeshMod_TypePolygon = 'P',
} MeshMod_Type;

typedef enum Mesh_SubType {
	MeshMod_SubTypeHash = 'H'
} MeshMod_SubType;

#define MESHMOD_TAG(a, b, c, d, e, f) (MeshMod_Tag)( \
		(uint64_t)(a) << 56 | (uint64_t)(b) << 40 | (uint64_t)(c) << 32 | (uint64_t)(d) << 24 | (uint64_t)(e) << 16 | (uint64_t)(f) << 8)
#define MESHMOD_VERTEXTAG(b, c, d, e, f) (MeshMod_Tag)(MESHMOD_TAG(MeshMod_TypeVertex, b, c, d, e, f))
#define MESHMOD_EDGETAG(b, c, d, e, f) (MeshMod_Tag)(MESHMOD_TAG(MeshMod_TypeEdge, b, c, d, e, f))
#define MESHMOD_POLYGONTAG(b, c, d, e, f) (MeshMod_Tag)(MESHMOD_TAG(MeshMod_TypePolygon, b, c, d, e, f))

#define MESHMOD_SUBTYPE_TAG(subtype, tag) (MeshMod_Tag)( (tag) | (uint64_t)(subtype) << 48)
#define MESHMOD_HASHTAG(tag) MESHMOD_SUBTYPE_TAG(MeshMod_SubTypeHash, tag)

// call these as you start and stop using meshmod, they setup a few globals safely
AL2O3_EXTERN_C void MeshMod_StartUp(void);
AL2O3_EXTERN_C void MeshMod_Shutdown(void);

#endif // GFX_MESHMOD_MESHMOD_H