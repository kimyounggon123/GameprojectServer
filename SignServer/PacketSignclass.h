#ifndef _PACKETSIGNCLASS_H
#define _PACKETSIGNCLASS_H

#include "PacketProcessclass.h"
#include "SignMessageclass.h"

class PacketSignclass
	:public PacketProcessclass
{
	SignMessageclass& sign_msg;


	std::map<PK_TYPE, std::function<ERROR_CODE(Packet*, SOCKETINFO*)>> process; // client -> server process
	std::function<ERROR_CODE(Packet*, SOCKETINFO*)> sign_up_try;
	std::function<ERROR_CODE(Packet*, SOCKETINFO*)> sign_in_try;
	ERROR_CODE sign_in_try_func(Packet* pk, SOCKETINFO*);
	ERROR_CODE sign_up_try_func(Packet* pk, SOCKETINFO*);

	std::map<PK_TYPE, std::function<ERROR_CODE(Packet*, SOCKET_AND_ADDR*)>> process_fromDB; // DB -> server process
	std::function<ERROR_CODE(Packet*, SOCKET_AND_ADDR*)> sign_up_complete;
	std::function<ERROR_CODE(Packet*, SOCKET_AND_ADDR*)> sign_in_complete;
	ERROR_CODE sign_in_success_func(Packet*, SOCKET_AND_ADDR*);
	ERROR_CODE sign_up_success_func(Packet*, SOCKET_AND_ADDR*);

	std::function<ERROR_CODE(Packet*, SOCKET_AND_ADDR*)> sign_up_fail;
	std::function<ERROR_CODE(Packet*, SOCKET_AND_ADDR*)> sign_in_fail;
	ERROR_CODE sign_in_fail_func(Packet*, SOCKET_AND_ADDR*);
	ERROR_CODE sign_up_fail_func(Packet*, SOCKET_AND_ADDR*);

	static PacketSignclass* instance;
	PacketSignclass():PacketProcessclass(), sign_msg(SignMessageclass::getInstance()) {}

public:
	static PacketSignclass& getInstance() {
		if (instance == nullptr) instance = new PacketSignclass;
		return *instance;
	}

	std::function<ERROR_CODE(Packet*, SOCKETINFO*)> get_func(PK_TYPE type);
	std::function<ERROR_CODE(Packet*, SOCKET_AND_ADDR*)> get_func_fromDB(PK_TYPE type);
	void initialize() override;
};
#endif

