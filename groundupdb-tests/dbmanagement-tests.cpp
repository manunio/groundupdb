//
// Created by maxx on 15/12/20.
//
#include "tests.h"

#include <filesystem>
#include <string>

#include "groundupdb/groundupdb.h"

namespace fs = std::filesystem;

TEST_CASE("Create a new empty database", "[CreateEmptyDatabase]")
{
	SECTION("Default Settings")
	{
		std::string dbname("myemptydb");
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::createEmptyDB(dbname));

		REQUIRE(fs::is_directory(fs::status(db->getDirectory())));

		const auto& p = fs::directory_iterator(db->getDirectory());
		REQUIRE(p == end(p));

		db->destroy();
		REQUIRE(!fs::exists(fs::status(db->getDirectory())));
	}
}

TEST_CASE("Load and existing database", "[loadDB]")
{
	SECTION("Default Settings")
	{
		std::string dbname("myemptydb");
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::createEmptyDB(dbname));

		std::unique_ptr<groundupdb::IDatabase> db2(groundupdb::GroundUpDB::loadDB(dbname));

		REQUIRE(fs::is_directory(fs::status(db2->getDirectory())));

		const auto& p = fs::directory_iterator(db2->getDirectory());
		REQUIRE(p == end(p));

		db2->destroy();
		REQUIRE(!fs::exists(fs::status(db2->getDirectory())));
	}
}