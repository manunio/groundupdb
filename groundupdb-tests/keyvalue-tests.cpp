//
// Created by maxx on 15/12/20.
//

#include "catch.hpp"
#include "groundupdb.h"
#include <filesystem>

namespace fs = std::filesystem;

TEST_CASE("Store and retrieve a value", "[setKeyValue, getKeyValue]")
{
	SECTION("Basic set and get")
	{
		std::string dbname("myemptydb");
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::createEmptyDB(dbname));

		std::string key("simplestring");
		std::string value("A test value");

		db->setKeyValue(key, value);
		REQUIRE(value == db->getKeyValue(key));

		std::string value2("A test value 2");
		db->setKeyValue(key, value2);
		REQUIRE(value2 == db->getKeyValue(key));


		db->destroy();
	}

	SECTION("Bucketed set and get")
	{
		std::string dbname("myemptydb");
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::createEmptyDB(dbname));

		std::string key("simplestring");
		std::string value("some highly valuable value");
		std::string bucket("bucket 1");

		db->setKeyValue(key, value, bucket);
		REQUIRE(value == db->getKeyValue(key));

		db->destroy();
	}
}

TEST_CASE("Store and retrieve a list", "setkeyValue(std::string, std::unordered_set<std::string>),getKeyValueSet")
{
	SECTION("Basic list set and get")
	{
		std::string dbname("myemptydb");
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::createEmptyDB(dbname));

		std::string key("simplestring");
		std::string v1("value 1");
		std::string v2("value 2");
		std::string v3("value 3");

		std::unordered_set<std::string> set;
		set.insert(v1);
		set.insert(v2);
		set.insert(v3);

		db->setKeyValue(key, set);
		auto result = db->getKeyValueSet(key);
		REQUIRE(result.get()->size() == 3);
		REQUIRE(result.get()->find(v1) != result.get()->end());
		REQUIRE(result.get()->find(v2) != result.get()->end());
		REQUIRE(result.get()->find(v3) != result.get()->end());

		db->destroy();
	}
}