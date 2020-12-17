//
// Created by maxx on 15/12/20.
//

#include "database.h"
#include "extensions/extdatabase.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <unordered_map>

namespace fs = std::filesystem;

using namespace groundupdb;
using namespace groundupdbext;

// 'Hidden' Database::Impl class here

class EmbeddedDatabase::Impl : public IDatabase
{
public:
	Impl(std::string dbname, std::string fullpath);
	~Impl();

	std::string getDirectory(void);

	//Key-Value use cases
	void setKeyValue(std::string key, std::string value);
	std::string getKeyValue(std::string key);

	//management functions
	static const std::unique_ptr<IDatabase> createEmpty(std::string dbname);
	static const std::unique_ptr<IDatabase> load(std::string dbname);
	void destroy();
private:
	std::string m_name;
	std::string m_fullpath;
	std::unordered_map<std::string, std::string> m_keyValueStore;
};

EmbeddedDatabase::Impl::Impl(std::string dbname, std::string fullpath)
	: m_name(dbname), m_fullpath(fullpath)
{
	for (auto& p : fs::directory_iterator(getDirectory()))
	{
		if (p.exists() && p.is_regular_file())
		{
			//check if extension is .kv
			if (".kv" == p.path().extension())
			{
				std::ifstream t(p.path());
				std::string value;
				std::string keyWithString = p.path().filename();
				//ASSUMPTION always ends with _string.kv
				std::string key = keyWithString.substr(0, keyWithString.length() - 10);  // DANGEROUS!!

				t.seekg(0, std::ios::end);
				value.reserve(t.tellg());
				t.seekg(0, std::ios::beg);

				value.assign((std::istreambuf_iterator<char>(t)),
					std::istreambuf_iterator<char>());

				m_keyValueStore.insert({ key, value });
			}
		}
	}
}

EmbeddedDatabase::Impl::~Impl()
{

}

//Management functions
const std::unique_ptr<IDatabase> EmbeddedDatabase::Impl::createEmpty(std::string dbname)
{
	std::string basedir(".groundupdb");
	if (!fs::exists(basedir))
		fs::create_directory(basedir);
	std::string dbfolder(basedir + "/" + dbname);
	if (!fs::exists(dbfolder))
		fs::create_directory(dbfolder);
	return std::make_unique<EmbeddedDatabase::Impl>(dbname, dbfolder);
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::Impl::load(std::string dbname)
{
	//here we assume everything exists..
	std::string basedir(".groundupdb");
	std::string dbfolder(basedir + "/" + dbname);
	return std::make_unique<EmbeddedDatabase::Impl>(dbname, dbfolder);
}

void EmbeddedDatabase::Impl::destroy()
{
	if (fs::exists(m_fullpath))
		fs::remove_all(m_fullpath);

	m_keyValueStore.clear();
}

//Instance users functions

std::string EmbeddedDatabase::Impl::getDirectory()
{
	return m_fullpath;
}

void EmbeddedDatabase::Impl::setKeyValue(std::string key, std::string value)
{
	std::ofstream os;
	os.open(m_fullpath + "/" + key + "_string.kv", std::ios::out | std::ios::trunc);
	os << value;
	os.close();
	m_keyValueStore.insert({ key, value });

}
std::string EmbeddedDatabase::Impl::getKeyValue(std::string key)
{
	const auto& v = m_keyValueStore.find(key);
	if (v == m_keyValueStore.end())
	{
		return ""; //DANGEROUS! Should be 'not found TODO: error handling
	}
	return v->second;

	/*
	std::ifstream t(m_fullpath + "/" + key + "_string.kv");
	std::string value;

	t.seekg(0, std::ios::end);
	value.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	value.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	return value;
	 */
}


// High level Database client API implementation below

// Embedded Database
EmbeddedDatabase::EmbeddedDatabase(std::string dbname, std::string fullpath)
	: mImpl(std::make_unique<EmbeddedDatabase::Impl>(dbname, fullpath))
{
	;
}

EmbeddedDatabase::~EmbeddedDatabase()
{
	;
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::load(std::string dbname)
{
	return EmbeddedDatabase::Impl::load(dbname);
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::createEmpty(std::string dbname)
{
	return EmbeddedDatabase::Impl::createEmpty(dbname);
}

void EmbeddedDatabase::destroy()
{
	mImpl->destroy();
}

// Instance users functions

std::string EmbeddedDatabase::getDirectory()
{
	return mImpl->getDirectory();
}

void EmbeddedDatabase::setKeyValue(std::string key, std::string value)
{
	mImpl->setKeyValue(key, value);
}

std::string EmbeddedDatabase::getKeyValue(std::string key)
{
	return mImpl->getKeyValue(key);
}
