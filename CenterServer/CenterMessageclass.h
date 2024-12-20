#ifndef _CENTERMESSAGECLASS_H
#define _CENTERMESSAGECLASS_H

#include "stdafx.h"
#include "SOCKETINFO.h"
#include "PacketMessageclass.h"

class CenterMessageclass
	:public PacketMessageclass
{
	static CenterMessageclass* instance;
	CenterMessageclass():PacketMessageclass(){}

public:
	static CenterMessageclass& getInstance() {
		if (instance == nullptr) instance = new CenterMessageclass;
		return *instance;
	}

	/// <run these in process class>
	void welcome_client(const char* ID, const SOCKETINFO& info);
	void bye_client(const char* ID, const SOCKETINFO& info);
	void client_msg(const char* ID, const char* chat, const SOCKETINFO& info);
	void some_goto_gameroom(const char* ID, const SOCKETINFO& info);
	/// </run these in process class>
};
#endif