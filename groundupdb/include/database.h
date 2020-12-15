//
// Created by maxx on 15/12/20.
//

#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Database
{
public:
	Database(std::string dbname, std::string fullpath);

	std::string getDirectory();

	//Key-Value use cases
	void setKeyValue(std::string key, std::string value);
	std::string getKeyValue(std::string key);

	//management functions
	static Database createEmpty(std::string dbname);
	void destroy();
	static Database loadDB(std::string dbname);
protected:
	std::string m_name;
	std::string m_fullpath;
	//TODO: std::pointer<DatabaseImpl> mImpl; C++11 IMPl IDIOM for forward binary compatibility.
};


#endif //DATABASE_H
