#ifndef _SIGNDBPROC_H
#define _SIGNDBPROC_H


#include "PacketProcessclass.h" // inherit
#include "UserInfoManager.h"

class SignDBproc
	:public PacketProcessclass
{
	UserInfoManager* manager;

	std::map<PK_TYPE, std::function<ERROR_CODE(Packet*)>> proc;
	std::function<ERROR_CODE(Packet*)> sign_in_proc;
	std::function<ERROR_CODE(Packet*)> sign_up_proc;



	ERROR_CODE client_sign_in_proc(Packet* pk);
	ERROR_CODE client_sign_up_proc(Packet* pk);
public:
	SignDBproc(const char* server, const char* user, const char* password, const char* database) : PacketProcessclass(),
		manager(new UserInfoManager(server, user, password, database))
	{}

	~SignDBproc() {
		delete manager;
		manager = nullptr;
	}

	void initialize();
	std::function<ERROR_CODE(Packet*)> get_func(PK_TYPE type);
};
#endif
