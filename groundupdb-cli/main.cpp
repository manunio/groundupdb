//
// Created by maxx on 15/12/20.
//
#include <iostream>
#include "cxxopts.hpp"
#include "groundupdb/groundupdb.h"

using namespace std;

cxxopts::Options options("groundupdb-cli", "CLI for GroundUpDB");

void printUsage()
{
	cout << "Opps! bad config." << endl;
}

int main(int argc, char* argv[])
{
	options.add_options()
		("c,create", "Create a DB")
		("d,destroy", "Destroy a DB")
		("s,set", "Set a key in a DB")
		("g,get", "Get a key from a DB")
		("q,query", "Query the DB (must also specify a query term. E.g. b for bucket)")
		("n,name", "IDatabase name (required)", cxxopts::value<std::string>())
		("k,key", "Key to set/get", cxxopts::value<std::string>())
		("v,value", "Value to set", cxxopts::value<std::string>());

	cxxopts::ParseResult result = options.parse(argc, argv);

	if (result.count("c") == 1)
	{
		if (result.count("n") == 0)
		{
			cout << "You must specify  a database name with -n <name>" << endl;
			printUsage();
			return 1;
		}
		//create database
		std::string dbname(result["n"].as<std::string>());
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::createEmptyDB(dbname));
		return 0;
	}

	if (result.count("s") == 1)
	{
		if (result.count("n") == 0)
		{
			cout << "You must specify  a database name with -n <name>" << endl;
			printUsage();
			return 1;
		}
		if (result.count("k") == 0)
		{
			cout << "You must specify  a key with -k <key>" << endl;
			printUsage();
			return 1;
		}
		if (result.count("v") == 0)
		{
			cout << "You must specify  a value with -v <value>" << endl;
			printUsage();
			return 1;
		}
		//Set key and value in database
		std::string dbname(result["n"].as<std::string>());
		std::string k(result["k"].as<std::string>());
		std::string v(result["v"].as<std::string>());
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::loadDB(dbname));
		db->setKeyValue(k, v);
		return 0;
	}

	if (result.count("g") == 1)
	{
		if (result.count("n") == 0)
		{
			cout << "You must specify  a database name with -n <name>" << endl;
			printUsage();
			return 1;
		}
		if (result.count("k") == 0)
		{
			cout << "You must specify  a key with -k <key>" << endl;
			printUsage();
			return 1;
		}
		//Get key value from database
		std::string dbname(result["n"].as<std::string>());
		std::string k(result["k"].as<std::string>());
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::loadDB(dbname));
		cout << db->getKeyValue(k) << endl;
		return 0;
	}

	if (result.count("d") == 1)
	{
		if (result.count("n") == 0)
		{
			cout << "You must specify  a database name with -n <name>" << endl;
			printUsage();
			return 1;
		}
		//destroy database
		std::string dbname(result["n"].as<std::string>());
		std::unique_ptr<groundupdb::IDatabase> db(groundupdb::GroundUpDB::loadDB(dbname));
		db->destroy();
		return 0;
	}

	cout << "No command specified!" << endl;
	printUsage();
	return 1;
};