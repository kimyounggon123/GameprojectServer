#include "DBmanager.h"

DBmanager::DBmanager(const char* server, const char* user, const char* password, const char* database):
	server(new char[strlen(server) + 1]),
	user(new char[strlen(user) + 1]),
	password(new char[strlen(password) + 1]),
	database(new char[strlen(database) + 1]),
	conn(nullptr), res(nullptr), row(NULL), isInitialized(false)
{
	strcpy_s(this->server, strlen(server) + 1, server);
	strcpy_s(this->user, strlen(user) + 1, user);
	strcpy_s(this->password, strlen(password) + 1, password);
	strcpy_s(this->database, strlen(database) + 1, database);
}
DBmanager::~DBmanager() {
	delete[] server; server = nullptr;
	delete[] user; user = nullptr;
	delete[] password; password = nullptr;
	delete[] database; database = nullptr;
	mysql_close(conn);
}
void DBmanager::initialize() {
	if (isInitialized) return;

	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
		exit(1);

	isInitialized = true;
}

QUERY_RESULT DBmanager::use_query(const char* query) {
	if (!isInitialized) return QUERY_RESULT::INITIALIZE_ERROR;

	if (mysql_query(conn, query)) return QUERY_RESULT::QUERY_ERROR;
	res = mysql_use_result(conn);
	mysql_free_result(res);
	return QUERY_RESULT::COMPLETE;
}