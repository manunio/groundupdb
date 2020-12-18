//
// Created by maxx on 17/12/20.
//
#include <iostream>

#include "catch.hpp"
#include "groundupdb/groundupdb.h"

TEST_CASE("Measure basic performance", "[setKeyValue, getKeyValue]")
{
	SECTION("Store and Retrieve 100 000 keys - Memory cache key-value store")
	{
		std::cout << "====== Default key-value store performance test ======" << std::endl;
		std::string dbname("myemptydb");
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::createEmptyDB(dbname));

		int total = 100000;

		// 1. Pre-generate the keys and values in memory (so we don't skew the test)
		std::unordered_map<std::string, std::string> keyValues;
		long i = 0;
		std::cout << "Pre-generating key value pairs...\n";
		for (; i < total; i++)
		{
			keyValues.emplace(std::to_string(i), std::to_string(i)); // C++11, uses std::forward
		}
		std::cout << "Key size is max " << std::to_string(total - 1).length() << " bytes\n";

		// 2. Store 100 000 key-value pairs (no overlap)
		// Raw storage speed
		std::cout << "======= SET =======\n";
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (auto& keyValue : keyValues)
		{
			db->setKeyValue(keyValue.first, keyValue.second);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << " " << keyValues.size() << " completed in "
				  << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0)
				  << " seconds\n";

		std::cout << " "
				  << (keyValues.size() * 1000000.0
					  / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
				  << " requests per second\n";
		std::cout << "\n";

		// 3. Retrieve 100 000 key-value pairs (no overlap) (use % 10)
		// Raw retrieval speed
		std::string aString("blank");
		std::string& result(aString);
		std::cout << "======= SET =======\n";
		begin = std::chrono::steady_clock::now();
		for (auto& keyValue: keyValues)
		{
			result = db->getKeyValue(keyValue.first);
		}
		end = std::chrono::steady_clock::now();
		std::cout << " " << keyValues.size() << " completed in "
				  << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0)
				  << " seconds\n";

		std::cout << " "
				  << (keyValues.size() * 1000000.0
					  / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
				  << " requests per second\n";
		std::cout << "\n";

		// 4. Retrieve the same 100 000 key-value pairs
		// Retrieval speed with a 'warm cache' (if any implemented)

		// 5. Store 500 000 key-value pair UPDATES (so half of the data is 'new') (first half of db)
		// So the performance of half of the key-value pairs may differ

		// 6. Retrieve the same 100 000 key-value pairs
		// Same as 4 if we have a 'write through cache', halfway between 4 and 3 otherwise

		// We know we have been successful when:-
		// We have min,max,var,total time for each group of operations
		// We have saved these results to a csv file for later comparison

		// 7. Tear down
		std::cout << "Tests complete\n";
		db->destroy();
	}

	SECTION("Store and retrieve 100 000 keys - In-memory key-value  store")
	{
		std::cout << "====== In-memory key-value store performance test ======" << std::endl;
		std::string dbname("myemptydb");
		std::unique_ptr<groundupdb::KeyValueStore> memoryStore = std::make_unique<groundupdbext::MemoryKeyValueStore>();
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::createEmptyDB(dbname, memoryStore));

		int total = 100000;

		// 1. Pre-generate the keys and values in memory (so we don't skew the test)
		std::unordered_map<std::string, std::string> keyValues;
		long i = 0;
		std::cout << "Pre-generating key value pairs...\n";
		for (; i < total; i++)
		{
			keyValues.emplace(std::to_string(i), std::to_string(i)); // C++11, uses std::forward
		}
		std::cout << "Key size is max " << std::to_string(total - 1).length() << " bytes\n";

		// 2. Store 100 000 key-value pairs (no overlap)
		// Raw storage speed
		std::cout << "======= SET =======\n";
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (auto& keyValue : keyValues)
		{
			db->setKeyValue(keyValue.first, keyValue.second);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << " " << keyValues.size() << " completed in "
				  << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0)
				  << " seconds\n";

		std::cout << " "
				  << (keyValues.size() * 1000000.0
					  / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
				  << " requests per second\n";
		std::cout << "\n";

		// 3. Retrieve 100 000 key-value pairs (no overlap) (use % 10)
		// Raw retrieval speed
		std::string aString("blank");
		std::string& result(aString);
		std::cout << "======= SET =======\n";
		begin = std::chrono::steady_clock::now();
		for (auto& keyValue: keyValues)
		{
			result = db->getKeyValue(keyValue.first);
		}
		end = std::chrono::steady_clock::now();
		std::cout << " " << keyValues.size() << " completed in "
				  << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0)
				  << " seconds\n";

		std::cout << " "
				  << (keyValues.size() * 1000000.0
					  / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
				  << " requests per second\n";
		std::cout << "\n";

		// Tear down
		std::cout << "Tests complete\n";
		db->destroy();
	}
		// Now do the same for pure disc backed storage

	SECTION("Store and retrieve 100 000 keys - File based key-value store")
	{
		std::cout << "====== File based key-value store performance test ======" << std::endl;
		std::string dbname("myemptydb");
		std::string fullpath = ".groundupdb/" + dbname;

		std::unique_ptr<groundupdb::KeyValueStore>
			fileStore = std::make_unique<groundupdbext::FileKeyValueStore>(fullpath);
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::createEmptyDB(dbname, fileStore));

		int total = 100000;

		// 1. Pre-generate the keys and values in memory (so we don't skew the test)
		std::unordered_map<std::string, std::string> keyValues;
		long i = 0;
		std::cout << "Pre-generating key value pairs...\n";
		for (; i < total; i++)
		{
			keyValues.emplace(std::to_string(i), std::to_string(i)); // C++11, uses std::forward
		}
		std::cout << "Key size is max " << std::to_string(total - 1).length() << " bytes\n";

		// 2. Store 100 000 key-value pairs (no overlap)
		// Raw storage speed
		std::cout << "======= SET =======\n";
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (auto& keyValue : keyValues)
		{
			db->setKeyValue(keyValue.first, keyValue.second);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		std::cout << " " << keyValues.size() << " completed in "
				  << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0)
				  << " seconds\n";

		std::cout << " "
				  << (keyValues.size() * 1000000.0
					  / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
				  << " requests per second\n";
		std::cout << "\n";

		// 3. Retrieve 100 000 key-value pairs (no overlap) (use % 10)
		// Raw retrieval speed
		std::string aString("blank");
		std::string& result(aString);
		std::cout << "======= SET =======\n";
		begin = std::chrono::steady_clock::now();
		for (auto& keyValue: keyValues)
		{
			result = db->getKeyValue(keyValue.first);
		}
		end = std::chrono::steady_clock::now();
		std::cout << " " << keyValues.size() << " completed in "
				  << (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0)
				  << " seconds\n";

		std::cout << " "
				  << (keyValues.size() * 1000000.0
					  / std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())
				  << " requests per second\n";
		std::cout << "\n";

		// Tear down
		std::cout << "Tests complete\n";
		db->destroy();

	}

}