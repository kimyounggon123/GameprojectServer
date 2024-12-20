#ifndef _SIGNMESSAGECLASS_H
#define _SIGNMESSAGECLASS_H


#include "PacketMessageclass.h"
class SignMessageclass
	: public PacketMessageclass
{
	static SignMessageclass* instance;
	SignMessageclass():PacketMessageclass(){}

	
	std::map<PK_TYPE, std::function<void(const SOCKET_AND_ADDR& info, const char* ID, const char* PW)>> sign_msg_map;

public:
	static SignMessageclass& getInstance() {
		if (instance == nullptr) instance = new SignMessageclass;
		return *instance;
	}

	// sign up
	void try_signup(const SOCKET_AND_ADDR& info, const char* ID, const char* PW);
	void signup_done(const SOCKET_AND_ADDR& info, const char* ID, const char* PW);
	void signup_fail(const SOCKET_AND_ADDR& info, const char* ID, const char* PW);
	// sign in
	void try_signin(const SOCKET_AND_ADDR& info, const char* ID, const char* PW);
	void signin_done(const SOCKET_AND_ADDR& info, const char* ID, const char* PW);
	void signin_fail(const SOCKET_AND_ADDR& info, const char* ID, const char* PW);

};
#endif