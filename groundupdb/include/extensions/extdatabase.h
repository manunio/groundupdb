//
// Created by maxx on 16/12/20.
//

#ifndef EXTDATABASE_H
#define EXTDATABASE_H

#include "database.h"

#include <memory>

namespace groundupdbext
{
	using namespace groundupdb;

	class EmbeddedDatabase : public IDatabase
	{
	public:
		EmbeddedDatabase(std::string dbname, std::string fullpath);
		~EmbeddedDatabase();

		std::string getDirectory();

		//Key-Value use cases
		void setKeyValue(std::string key, std::string value);
		std::string getKeyValue(std::string key);

		//management functions
		static const std::unique_ptr<IDatabase> createEmpty(std::string dbname);
		static const std::unique_ptr<IDatabase> load(std::string dbname);
		void destroy();

		class Impl;

	private:
		std::unique_ptr<Impl> mImpl;
	};
}

#endif //EXTDATABASE_H
