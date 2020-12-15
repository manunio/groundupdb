//
// Created by maxx on 15/12/20.
//
#include "groundupdb.h"

GroundUpDB::GroundUpDB(){

}
Database GroundUpDB::createEmptyDB(std::string &dbname)
{
	return Database::createEmpty(dbname);
}
