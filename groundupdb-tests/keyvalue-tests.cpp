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

		db->destroy();
		REQUIRE(!fs::exists(fs::status(db->getDirectory())));
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