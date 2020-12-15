//
// Created by maxx on 15/12/20.
//
#include "tests.h"

#include <filesystem>
#include <string>

#include "groundupdb.h"

namespace fs = std::filesystem;

TEST_CASE("Create a new empty database", "[CreateEmptyDatabase]") {
	SECTION("Default Settings") {
		std::string dbname("myemptydb");
		Database db(GroundUpDB::createEmptyDB(dbname));

		REQUIRE(fs::is_directory(fs::status(db.getDirectory())));

		const auto& p= fs::directory_iterator(db.getDirectory());
		REQUIRE(p == end(p));

		db.destroy();
		REQUIRE(!fs::exists(fs::status(db.getDirectory())));
	}
}
