#ifndef _SIGNCLASS_H
#define _SIGNCLASS_H

#include "stdafx.h"
#include "UserList.h"

#include "Packet.h"
class SignClass
{
	User* you;

	ERROR_CODE do_sign_in(char* ID, char* PW);
	ERROR_CODE do_sign_up(char* ID, char* PW);

	static SignClass* instance;
	SignClass(): you(nullptr)
	{}
public:
	static SignClass& getInstance() {
		if (instance == nullptr) instance = new SignClass;
		return *instance;
	}
	~SignClass()
	{
		delete you;
		you = nullptr;
	}

	ERROR_CODE select_sign_type(PK_TYPE* type, char* ID, char* PW);
	User* getYou() const { return you; }

	void make_you(const char* name) {
		you = new User(name);
	}
};
#endif
