#ifndef _USERINFOMANAGER_H
#define _USERINFOMANAGER_H

#include "DBmanager.h"
#include "SOCKETINFO.h"
class UserInfoManager
	:public DBmanager
{
public:
	UserInfoManager(const char* server, const char* user, const char* password, const char* database)
		: DBmanager(server, user, password, database)
	{}
	~UserInfoManager() {}

	QUERY_RESULT add_new_user(std::string ID, std::string password); // sign up
	QUERY_RESULT check_user_exists(std::string ID, std::string password); // sign in
	QUERY_RESULT write_sign_in_log(std::string ID, const SOCKET_AND_ADDR& client_sockinfo); // sign in log

	QUERY_RESULT show_table();

};
#endif

