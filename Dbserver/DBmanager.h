#ifndef _DBMANAGER_H
#define _DBMANAGER_H

#include "mysql.h"
#include "stdafx.h"


enum class QUERY_RESULT {
	// Error result
	INITIALIZE_ERROR,
	QUERY_ERROR,


	// Find result
	NO_RESULT,
	COMPLETE
};
class DBmanager
{
protected:
	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;

	char* server;
	char* user;
	char* password;
	char* database;

	bool isInitialized;
public:
	DBmanager(const char* server, const char* user, const char* password, const char* database);
	virtual ~DBmanager();

	void initialize();

	QUERY_RESULT use_query(const char* query);
};
#endif
