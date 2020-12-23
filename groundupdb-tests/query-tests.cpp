//
// Created by maxx on 23/12/20.
//
#include "catch.hpp"
#include "groundupdb/groundupdb.h"

#include <iostream>

using namespace groundupdb;
TEST_CASE("Basic query tests", "[query]")
{

//   [Who]   As a database user
//   [What]  I want to be able to logically segment my data
//   [Value] To make storage, querying, and retrieval easier and quicker
	SECTION("Bucketed set and get")
	{
		std::cout << "Creating DB" << std::endl;
		std::string dbname("myemptydb");
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::createEmptyDB(dbname));

// We know we have been successful when:-
// 1. The retrieved value is the same as the stored value
		std::cout << "Setting keys" << std::endl;
		std::string key("simplestring");
		std::string value("Some highly valuable value");
		std::string key2("second string");
		std::string value2("second valuable value");
		std::string key3("third string");
		std::string value3("third valuable value");
		std::string key4("fourth string");
		std::string value4("fourth valuable value");
		std::string bucket("bucket 1");
		db->setKeyValue(key, value, bucket);
		db->setKeyValue(key2, value2);
		db->setKeyValue(key3, value3);
		db->setKeyValue(key4, value4, bucket);

// Note set with bucket proven in keyvalue-tests.cpp

// Now add the bucket query
		std::cout << "Creating query" << std::endl;
		std::string queryBucket("bucket 1"); // second definition to ensure no weird pointer serialisation
		groundupdb::BucketQuery bq(queryBucket);
		std::cout << "Executing query" << std::endl;
		std::unique_ptr<groundupdb::IQueryResult> res = db->query(bq);
		std::cout << "Retrieving results" << std::endl;
		std::unique_ptr<std::unordered_set<std::string>> recordKeys(res->recordKeys());

		for (auto it = recordKeys.get()->begin(); it != recordKeys.get()->end(); it++)
		{
			INFO("  Key in bucket: " << *it);
		}
		INFO("  First key: " << *recordKeys.get()->begin());

		std::cout << "Asserting size" << std::endl;
		REQUIRE(recordKeys.get()->size() == 2);
		std::cout << "Asserting key 1 found" << std::endl;
		REQUIRE(recordKeys.get()->find(key) != recordKeys.get()->end());
		std::cout << "Asserting key 4 found" << std::endl;
		REQUIRE(recordKeys.get()->find(key4) != recordKeys.get()->end());

		std::cout << "Destroying db" << std::endl;
		db->destroy();
	}
}