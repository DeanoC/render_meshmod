#pragma once

#include "al2o3_platform/platform.h"
#include "render_meshmod/handles.h"

// the top byte is the MeshMod_Type (ascii 'V', 'E', 'P')
// the next byte can be used as sub type data (currently ascii 'H' for hash) or just part of the tag
// the bottom byte of a tag are ignored by the registry and can be used to hold user data
typedef uint64_t MeshMod_Tag;
typedef struct { uint64_t tag; } MeshMod_VertexTag;
typedef struct { uint64_t tag; } MeshMod_EdgeTag;
typedef struct { uint64_t tag; } MeshMod_PolygonTag;

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
#define MESHMOD_VERTEXTAG(b, c, d, e, f) (MeshMod_VertexTag){ (MESHMOD_TAG(MeshMod_TypeVertex, b, c, d, e, f)) }
#define MESHMOD_EDGETAG(b, c, d, e, f) (MeshMod_EdgeTag){ (MESHMOD_TAG(MeshMod_TypeEdge, b, c, d, e, f)) }
#define MESHMOD_POLYGONTAG(b, c, d, e, f) (MeshMod_PolygonTag){ (MESHMOD_TAG(MeshMod_TypePolygon, b, c, d, e, f)) }

#define MESHMOD_SUBTYPE_TAG(subtype) ((uint64_t)(subtype) << 48ull)
AL2O3_FORCE_INLINE MeshMod_Tag MeshMod_VertexTagToHashTag(MeshMod_VertexTag vtag) {
	return ((vtag.tag & ~(0xFFull<<48ull)) | MESHMOD_SUBTYPE_TAG(MeshMod_SubTypeHash));
}
AL2O3_FORCE_INLINE MeshMod_Tag MeshMod_EdgeTagToHashTag(MeshMod_EdgeTag etag) {
	return ((etag.tag & ~(0xFFull<<48ull)) | MESHMOD_SUBTYPE_TAG(MeshMod_SubTypeHash));
}
AL2O3_FORCE_INLINE MeshMod_Tag MeshMod_PolygonTagToHashTag(MeshMod_PolygonTag ptag) {
	return ((ptag.tag & ~(0xFFull<<48ull)) | MESHMOD_SUBTYPE_TAG(MeshMod_SubTypeHash));
}

AL2O3_FORCE_INLINE MeshMod_VertexTag MeshMod_AddUserDataToVertexTag(MeshMod_VertexTag vtag, uint8_t userData) {
	MeshMod_VertexTag ret = { ((vtag.tag & ~(0xFFull<<48ull)) | userData ) };
	return ret;
}
AL2O3_FORCE_INLINE MeshMod_EdgeTag MeshMod_AddUserDataToEdgeTag(MeshMod_EdgeTag etag, uint8_t userData) {
	MeshMod_EdgeTag ret = { ((etag.tag & ~(0xFFull<<48ull)) | userData ) };
	return ret;
}
AL2O3_FORCE_INLINE MeshMod_PolygonTag MeshMod_AddUserDataToPolygonTag(MeshMod_PolygonTag ptag, uint8_t userData) {
	MeshMod_PolygonTag ret = { ((ptag.tag & ~(0xFFull<<48ull)) | userData ) };
	return ret;
}

// call these as you start and stop using meshmod, they setup a few globals safely
AL2O3_EXTERN_C void MeshMod_StartUp(void);
AL2O3_EXTERN_C void MeshMod_Shutdown(void);
