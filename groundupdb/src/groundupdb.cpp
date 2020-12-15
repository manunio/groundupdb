//
// Created by maxx on 15/12/20.
//
#include "groundupdb.h"

groundupdb::GroundUpDB::GroundUpDB(){

}
groundupdb::Database groundupdb::GroundUpDB::createEmptyDB(std::string &dbname)
{
	return Database::createEmpty(dbname);
}
groundupdb::Database groundupdb::GroundUpDB::loadDB(std::string& dbname)
{
	return Database::loadDB(dbname);
}
