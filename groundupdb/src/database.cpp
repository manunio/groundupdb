//
// Created by maxx on 15/12/20.
//

#include "database.h"

Database::Database(std::string dbname, std::string fullpath)
{

}
Database Database::createEmpty(std::string dbname)
{
	return Database("Max","Max");
}

std::string Database::getDirectory()
{
	return "Max";
}
