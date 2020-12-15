//
// Created by maxx on 15/12/20.
//

#include "database.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

groundupdb::Database::Database(std::string dbname, std::string fullpath)
	: m_name(dbname), m_fullpath(fullpath)
{

}
//Management functions
groundupdb::Database groundupdb::Database::createEmpty(std::string dbname)
{
	std::string basedir(".groundupdb");
	if (!fs::exists(basedir))
		fs::create_directory(basedir);
	std::string dbfolder(basedir + "/" + dbname);
	if (!fs::exists(dbfolder))
		fs::create_directory(dbfolder);
	return Database(dbname, dbfolder);
}

void groundupdb::Database::destroy()
{
	if (fs::exists(m_fullpath))
		fs::remove_all(m_fullpath);
}

groundupdb::Database groundupdb::Database::loadDB(std::string dbname)
{
	//here we assume everything exists..
	std::string basedir(".groundupdb");
	std::string dbfolder(basedir + "/" + dbname);
	return Database(dbname, dbfolder);

}

//Instance functions
std::string groundupdb::Database::getDirectory()
{
	return m_fullpath;
}

void groundupdb::Database::setKeyValue(std::string key, std::string value)
{
	std::ofstream os;
	os.open(m_fullpath + "/" + key + "_string.kv", std::ios::out | std::ios::trunc);
	os << value;
	os.close();

}
std::string groundupdb::Database::getKeyValue(std::string key)
{
	std::ifstream t(m_fullpath + "/" + key + "_string.kv");
	std::string value;

	t.seekg(0, std::ios::end);
	value.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	value.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	return value;
}