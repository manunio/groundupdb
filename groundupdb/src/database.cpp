//
// Created by maxx on 15/12/20.
//

#include "database.h"
#include "query.h"
#include "extensions/extdatabase.h"
#include "extensions/extquery.h"

#include <iostream>
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
	Impl(std::string dbname, std::string fullpath,
		std::unique_ptr<KeyValueStore>& kvStore);
	~Impl();

	std::string getDirectory(void);

	//Key-Value use cases
	void setKeyValue(std::string key, std::string value);
	void setKeyValue(std::string key, std::string value, std::string bucket);
	void setKeyValue(std::string key, std::unordered_set<std::string> value);
	std::string getKeyValue(std::string key);
	std::unique_ptr<std::unordered_set<std::string>> getKeyValueSet(std::string key);

	//Query records functions
	std::unique_ptr<IQueryResult> query(Query& query) const;
	std::unique_ptr<IQueryResult> query(BucketQuery& query) const;

	//management functions
	static const std::unique_ptr<IDatabase> createEmpty(std::string dbname);
	static const std::unique_ptr<IDatabase> createEmpty(std::string dbname, std::unique_ptr<KeyValueStore>& kvStore);
	static const std::unique_ptr<IDatabase> load(std::string dbname);
	void destroy();

private:
	std::string m_name;
	std::string m_fullpath;
	std::unique_ptr<KeyValueStore> m_keyValueStore;
	std::unique_ptr<KeyValueStore> m_indexStore;
};

EmbeddedDatabase::Impl::Impl(std::string dbname, std::string fullpath)
	: m_name(dbname), m_fullpath(fullpath)
{
	// Explicitly specify base type so it matches the make_unique expected class (KeyValueStore)
	std::unique_ptr<KeyValueStore> fileStore = std::make_unique<FileKeyValueStore>(fullpath);
	std::unique_ptr<KeyValueStore> memoryStore = std::make_unique<MemoryKeyValueStore>(fileStore);
	m_keyValueStore = std::move(memoryStore);

	//Explicitly specify base type so it matches the make_unique expected class (KeyValueStore)
	std::unique_ptr<KeyValueStore> fileIndexStore = std::make_unique<FileKeyValueStore>(fullpath + "/.indexes");
	std::unique_ptr<KeyValueStore> memoryIndexStore = std::make_unique<MemoryKeyValueStore>(fileIndexStore);
	m_indexStore = std::move(memoryIndexStore);
}

EmbeddedDatabase::Impl::Impl(std::string dbname, std::string fullpath, std::unique_ptr<KeyValueStore>& kvStore)
	: m_name(dbname), m_fullpath(fullpath), m_keyValueStore(kvStore.release())
{
	//Explicitly specify base type so it matches the make_unique expected class (KeyValueStore)
	std::unique_ptr<KeyValueStore> fileIndexStore = std::make_unique<FileKeyValueStore>(fullpath + "/.indexes");
	std::unique_ptr<KeyValueStore> memoryIndexStore = std::make_unique<MemoryKeyValueStore>(fileIndexStore);
	m_indexStore = std::move(memoryIndexStore);
}

EmbeddedDatabase::Impl::~Impl()
{
	; // [Optional] Flush the latest known state to disc here
}

//Management functions
const std::unique_ptr<IDatabase> EmbeddedDatabase::Impl::createEmpty(std::string dbname)
{
	std::string basedir(".groundupdb");
	if (!fs::exists(basedir))
		fs::create_directory(basedir);
	std::string dbfolder(basedir + "/" + dbname);
	return std::make_unique<EmbeddedDatabase::Impl>(dbname, dbfolder);
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::Impl::createEmpty(std::string dbname,
	std::unique_ptr<KeyValueStore>& kvStore)
{
	std::string basedir(".groundupdb");
	if (!fs::exists(basedir))
		fs::create_directory(basedir);
	std::string dbfolder(basedir + "/" + dbname);
	return std::make_unique<EmbeddedDatabase::Impl>(dbname, dbfolder, kvStore);
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
	m_keyValueStore->clear();
}

//Instance users functions

std::string EmbeddedDatabase::Impl::getDirectory()
{
	return m_fullpath;
}

void EmbeddedDatabase::Impl::setKeyValue(std::string key, std::string value)
{
	m_keyValueStore->setKeyValue(key, value);

}
std::string EmbeddedDatabase::Impl::getKeyValue(std::string key)
{
	return m_keyValueStore->getKeyValue(key);
}

void EmbeddedDatabase::Impl::setKeyValue(std::string key, std::string value, std::string bucket)
{
	setKeyValue(key, value);
	//Add to bucket index
	std::string idxkey("bucket::" + bucket);
	//Query the key index
	std::unique_ptr<std::unordered_set<std::string>> recordKeys(m_indexStore->getKeyValueSet(idxkey));
	recordKeys->insert(key);
	m_indexStore->setKeyValue(idxkey, *recordKeys.release()); // TODO: do we need this? Yes - may not be in memory store
	// TODO: replace the above with appendKeyValueSet(key,value)

}

void EmbeddedDatabase::Impl::setKeyValue(std::string key, std::unordered_set<std::string> value)
{
	m_keyValueStore->setKeyValue(key, value);
}

std::unique_ptr<std::unordered_set<std::string>> EmbeddedDatabase::Impl::getKeyValueSet(std::string key)
{
	return m_keyValueStore->getKeyValueSet(key);
}

//Query functions
std::unique_ptr<IQueryResult> EmbeddedDatabase::Impl::query(Query& q) const
{
	//return std::make_unique<DefaultResutl>(); //no result
	//Query is abstract, so try overloading here
	return query(static_cast<decltype(q)>(q));
}

std::unique_ptr<IQueryResult> EmbeddedDatabase::Impl::query(BucketQuery& query) const
{
	// Bucket query
	// construct a name for our key index
	std::string idxkey("bucket::" + query.bucket());

	//query the key index
	std::unique_ptr<IQueryResult> r = std::make_unique<DefaultQueryResult>(m_indexStore->getKeyValueSet(idxkey));
	return std::move(r);

}
// High level Database client API implementation below

// Embedded Database
EmbeddedDatabase::EmbeddedDatabase(std::string dbname, std::string fullpath)
	: mImpl(std::make_unique<EmbeddedDatabase::Impl>(dbname, fullpath))
{
	;
}

EmbeddedDatabase::EmbeddedDatabase(std::string dbname, std::string fullpath, std::unique_ptr<KeyValueStore>& kvStore)
	: mImpl(std::make_unique<EmbeddedDatabase::Impl>(dbname, fullpath, kvStore))
{
	;
}

EmbeddedDatabase::~EmbeddedDatabase()
{
	;
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::createEmpty(std::string dbname)
{
	return EmbeddedDatabase::Impl::createEmpty(dbname);
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::createEmpty(std::string dbname,
	std::unique_ptr<KeyValueStore>& kvStore)
{
	return EmbeddedDatabase::Impl::createEmpty(dbname, kvStore);
}

const std::unique_ptr<IDatabase> EmbeddedDatabase::load(std::string dbname)
{
	return EmbeddedDatabase::Impl::load(dbname);
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

void EmbeddedDatabase::setKeyValue(std::string key, std::string value, std::string bucket)
{
	mImpl->setKeyValue(key, value, bucket);
}
void EmbeddedDatabase::setKeyValue(std::string key, std::unordered_set<std::string> value)
{
	mImpl->setKeyValue(key, value);
}
std::string EmbeddedDatabase::getKeyValue(std::string key)
{
	return mImpl->getKeyValue(key);
}

std::unique_ptr<std::unordered_set<std::string>> EmbeddedDatabase::getKeyValueSet(std::string key)
{
	return mImpl->getKeyValueSet(key);
}

std::unique_ptr<IQueryResult> EmbeddedDatabase::query(Query& query) const
{
	return mImpl->query(query);
}

std::unique_ptr<IQueryResult> EmbeddedDatabase::query(BucketQuery& query) const
{
	return mImpl->query(query);

}
