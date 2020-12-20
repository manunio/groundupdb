//
// Created by maxx on 15/12/20.
//

#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <memory>
#include <functional>
#include "query.h"

namespace groundupdb
{

	class Store
	{
	public:
		Store() = default;
		virtual ~Store() = default;
	};

	class KeyValueStore : public Store
	{
	public:
		KeyValueStore() = default;
		virtual ~KeyValueStore() = default;

		//Key-Value user functions
		virtual void setKeyValue(std::string key, std::string value) = 0;
		virtual void setKeyValue(std::string key, std::string value, std::string bucket) = 0;
		virtual void setKeyValue(std::string key, std::unordered_set<std::string> value) = 0;
		virtual std::string getKeyValue(std::string key) = 0;

		virtual Set getKeyValueSet(std::string key) = 0;

		//Key-Value management functions
		virtual void loadKeysInfo(std::function<void(std::string key, std::string value)> callback) = 0;
		virtual void clear() = 0;
	};

	class IDatabase
	{
	public:
		IDatabase() = default;
		virtual ~IDatabase() = default;

		virtual std::string getDirectory() = 0;

		//Key-Value use cases
		virtual void setKeyValue(std::string key, std::string value) = 0;
		virtual void setKeyValue(std::string key, std::string value, std::string bucket) = 0;
		virtual void setKeyValue(std::string key, std::unordered_set<std::string>value) = 0;

		virtual std::string getKeyValue(std::string key) = 0;

		virtual Set getKeyValueSet(std::string key);

		// Query records functions
		virtual std::unique_ptr<IQueryResult> query(Query& query) const = 0;
		// TODO: replace below with generic polymorphic function
		virtual std::unique_ptr<IQueryResult> query(BucketQuery& query) const = 0;

		//management functions
		static const std::unique_ptr<IDatabase> createEmpty(std::string dbname);
		static const std::unique_ptr<IDatabase> load(std::string dbname);
		virtual void destroy() = 0;

	};
}

#endif //DATABASE_H
