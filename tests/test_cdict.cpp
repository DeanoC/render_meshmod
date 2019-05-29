#include "al2o3_catch2/catch2.hpp"
#include "gfx_meshmod/cdict.h"

#define CDICT_TEST(postfix, type) \
TEST_CASE("CDict" #postfix " create/destroy", "[MeshMod CDict]") { \
	MeshMod_CDict##postfix##Handle handle = MeshMod_CDict##postfix##Create(); \
	REQUIRE(handle); \
	MeshMod_CDict##postfix##Destroy(handle); \
} \
TEST_CASE("CDict" #postfix " add/lookup", "[MeshMod CDict]") { \
	MeshMod_CDict##postfix##Handle handle = MeshMod_CDict##postfix##Create(); \
	REQUIRE(handle); \
	REQUIRE(MeshMod_CDict##postfix##Add(handle, 0, 50)); \
	REQUIRE(MeshMod_CDict##postfix##KeyExists(handle, 0)); \
	REQUIRE(MeshMod_CDict##postfix##Add(handle, 10, 60)); \
	REQUIRE(MeshMod_CDict##postfix##KeyExists(handle, 10)); \
	REQUIRE(MeshMod_CDict##postfix##Add(handle, 5, 70)); \
	REQUIRE(MeshMod_CDict##postfix##KeyExists(handle, 5)); \
	type r50 = 0, r60 = 0, r70 = 0; \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 0, &r50)); \
	REQUIRE(r50 == 50); \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 10, &r60)); \
	REQUIRE(r60 == 60); \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 5, &r70)); \
	REQUIRE(r70 == 70); \
	r50 = 0, r60 = 0, r70 = 0; \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 5, &r70)); \
	REQUIRE(r70 == 70); \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 0, &r50)); \
	REQUIRE(r50 == 50); \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 10, &r60)); \
	REQUIRE(r60 == 60); \
	MeshMod_CDict##postfix##Destroy(handle); \
} \
TEST_CASE("CDict" #postfix " remove", "[MeshMod CDict]") { \
	MeshMod_CDict##postfix##Handle handle = MeshMod_CDict##postfix##Create(); \
	REQUIRE(handle); \
	REQUIRE(MeshMod_CDict##postfix##Add(handle, 0, 50)); \
	REQUIRE(MeshMod_CDict##postfix##Add(handle, 10, 60)); \
	REQUIRE(MeshMod_CDict##postfix##Add(handle, 5, 70)); \
	MeshMod_CDict##postfix##Remove(handle, 10); \
	REQUIRE(!MeshMod_CDict##postfix##KeyExists(handle, 10)); \
	REQUIRE(MeshMod_CDict##postfix##KeyExists(handle, 0)); \
	REQUIRE(MeshMod_CDict##postfix##KeyExists(handle, 5)); \
	type r50 = 0, r70 = 0; \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 0, &r50)); \
	REQUIRE(r50 == 50); \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 5, &r70)); \
	REQUIRE(r70 == 70); \
	MeshMod_CDict##postfix##Remove(handle, 0); \
	REQUIRE(!MeshMod_CDict##postfix##KeyExists(handle, 0)); \
	MeshMod_CDict##postfix##Remove(handle, 5); \
	REQUIRE(!MeshMod_CDict##postfix##KeyExists(handle, 5)); \
	REQUIRE(MeshMod_CDict##postfix##IsEmpty(handle)); \
} \
TEST_CASE("CDict" #postfix " reserve", "[MeshMod CDict]") { \
	MeshMod_CDict##postfix##Handle handle = MeshMod_CDict##postfix##Create(); \
	REQUIRE(handle); \
	REQUIRE(MeshMod_CDict##postfix##Add(handle, 0, 50)); \
	REQUIRE(MeshMod_CDict##postfix##KeyExists(handle, 0)); \
	REQUIRE(MeshMod_CDict##postfix##Add(handle, 10, 60)); \
	REQUIRE(MeshMod_CDict##postfix##KeyExists(handle, 10)); \
	REQUIRE(MeshMod_CDict##postfix##Add(handle, 5, 70)); \
	REQUIRE(MeshMod_CDict##postfix##KeyExists(handle, 5)); \
	MeshMod_CDict##postfix##Reserve(handle, 100);\
	REQUIRE(MeshMod_CDict##postfix##Capacity(handle) >= 100); \
	type r50 = 0, r60 = 0, r70 = 0; \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 0, &r50)); \
	REQUIRE(r50 == 50); \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 10, &r60)); \
	REQUIRE(r60 == 60); \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 5, &r70)); \
	REQUIRE(r70 == 70); \
	r50 = 0, r60 = 0, r70 = 0; \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 5, &r70)); \
	REQUIRE(r70 == 70); \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 0, &r50)); \
	REQUIRE(r50 == 50); \
	REQUIRE(MeshMod_CDict##postfix##Lookup(handle, 10, &r60)); \
	REQUIRE(r60 == 60); \
	MeshMod_CDict##postfix##Destroy(handle); \
} 


CDICT_TEST(U32, uint32_t)
CDICT_TEST(U64, uint64_t)


TEST_CASE("CDictU64 64 bit add/lookup", "[MeshMod CDict]") {
	MeshMod_CDictU64Handle handle = MeshMod_CDictU64Create();
	REQUIRE(handle);
	uint64_t r50 = 0, r60 = 0, r70 = 0;
	REQUIRE(MeshMod_CDictU64Add(handle, 0x8888888822222220U, 0x8888888822222250U));
	REQUIRE(MeshMod_CDictU64KeyExists(handle, 0x8888888822222220U));
	REQUIRE(MeshMod_CDictU64Lookup(handle, 0x8888888822222220U, &r50));
	REQUIRE(r50 == 0x8888888822222250U);

	REQUIRE(MeshMod_CDictU64Add(handle, 0x8888888822222210U, 0x8888888822222260U));
	REQUIRE(MeshMod_CDictU64KeyExists(handle, 0x8888888822222210U));
	REQUIRE(MeshMod_CDictU64Lookup(handle, 0x8888888822222210U, &r60));
	REQUIRE(r60 == 0x8888888822222260U);

	REQUIRE(MeshMod_CDictU64Add(handle, 0x8888888822222225U, 0x8888888822222270U));
	REQUIRE(MeshMod_CDictU64KeyExists(handle, 0x8888888822222225U));
	REQUIRE(MeshMod_CDictU64Lookup(handle, 0x8888888822222225U, &r70));
	REQUIRE(r70 == 0x8888888822222270U);

	r50 = 0, r60 = 0, r70 = 0;
	REQUIRE(MeshMod_CDictU64Lookup(handle, 0x8888888822222225U, &r70));
	REQUIRE(r70 == 0x8888888822222270U);
	REQUIRE(MeshMod_CDictU64Lookup(handle, 0x8888888822222220U, &r50));
	REQUIRE(r50 == 0x8888888822222250U);
	REQUIRE(MeshMod_CDictU64Lookup(handle, 0x8888888822222210U, &r60));
	REQUIRE(r60 == 0x8888888822222260U);


	r50 = 0, r60 = 0, r70 = 0;
	REQUIRE(MeshMod_CDictU64Lookup(handle, 0x8888888822222225U, &r70));
	REQUIRE(r70 == 0x8888888822222270U);
	MeshMod_CDictU64Remove(handle, 0x8888888822222225U);
	REQUIRE(MeshMod_CDictU64Lookup(handle, 0x8888888822222220U, &r50));
	REQUIRE(r50 == 0x8888888822222250U);
	MeshMod_CDictU64Remove(handle, 0x8888888822222220U);
	REQUIRE(MeshMod_CDictU64Lookup(handle, 0x8888888822222210U, &r60));
	REQUIRE(r60 == 0x8888888822222260U);

	MeshMod_CDictU64Destroy(handle);
}