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
		std::cout << "creating db" << std::endl;
		std::string dbname("myemptydb");
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::createEmptyDB(dbname));
		// We know we have been successful when:-
		// 1. We have a valid database reference returned
		//   - No test -> The above would have errored
		// 2. The DB has a folder that exists on the file system
		std::cout << "checking db directory" << std::endl;
		REQUIRE(fs::is_directory(fs::status(db->getDirectory())));
		// C++17 Ref: https://en.cppreference.com/w/cpp/filesystem/is_directory

		// 3. The database folder is empty (no database files yet)

		std::cout << "iterating over db directory" << std::endl;
		const auto& p = fs::directory_iterator(db->getDirectory()); // i.e. no contents as iterator is at the end already
		// C++17 Ref: https://en.cppreference.com/w/cpp/filesystem/directory_iterator

		REQUIRE(p == end(p));

		std::cout << "destroying db" << std::endl;
		db->destroy();
		std::cout << "checking destroyed db directory" << std::endl;
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