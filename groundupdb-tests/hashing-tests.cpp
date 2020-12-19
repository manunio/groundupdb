//
// Created by maxx on 19/12/20.
//

#include "catch.hpp"

#include "groundupdb/groundupdb.h"
#include "groundupdb/groundupdbext.h"
#include "highwayhash/highwayhash.h"

#include <unordered_map>

TEST_CASE("Test unordered_map custom hashing algorithms", "[set, get]")
{
	SECTION("Produces known expected value from known input")
	{
		groundupdbext::HighwayHash h;
		std::string text("Known");
		std::size_t r = h(text);

		using namespace highwayhash;
		const HHKey key HH_ALIGNAS(64) = { 1, 2, 3, 4 };
		HHResult64 result;
		HHStateT<HH_TARGET> state(key);
		HighwayHashT(&state, text.c_str(), text.length(), &result);
		REQUIRE(result == r);
	}

	SECTION("Previous hash does not affect next hash result")
	{
		groundupdbext::HighwayHash hfirst;
		std::size_t rfirst = hfirst("OtherThing");

		groundupdbext::HighwayHash h;
		std::size_t r1 = h("Known");
		std::size_t r2 = h("OtherThing");

		REQUIRE(r1 != r2);
		REQUIRE(r1 != rfirst);
		REQUIRE(rfirst == r2);

	}

	SECTION("Different seed keys produce different results for the same input")
	{
		groundupdbext::HighwayHash h1{ 1, 2, 3, 4 };
		std::size_t r1 = h1("AThing");

		groundupdbext::HighwayHash h2{ 5, 6, 7, 8 };
		std::size_t r2 = h2("AThing");

		REQUIRE(r1 != r2);
	}

	SECTION("Unordered map works as expected with custom hash")
	{
		std::unordered_map<std::string, std::string, groundupdbext::HighwayHash> m;
		std::string k1("A k1");
		std::string v1("A short v1.");
		std::string k2("Some k1");
		std::string v2("Some short v1.");
		m.emplace(k1, v1);
		m.emplace(k2, v2);

		REQUIRE(m[k1] == v1);
		REQUIRE(m[k2] == v2);

	}
}