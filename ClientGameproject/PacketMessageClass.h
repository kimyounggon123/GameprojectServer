#ifndef _PACKETMESSAGECLASS_H
#define _PACKETMESSAGECLASS_H

#include "stdafx.h"

class PacketMessageClass
{
	static PacketMessageClass* instance;
	PacketMessageClass(){}

	void print_time();
public:
	static PacketMessageClass& getInstance() {
		if (instance == nullptr) instance = new PacketMessageClass;
		return *instance;
	}

	/// <run these in PacketProcessclass>
	void welcome_client(const char* ID);
	void bye_client(const char* ID);
	void someone_chat(const char* ID, const char* chat);
	void someone_move(const char* ID, const char* pos_buf);
	/// </run these in PacketProcessclass>
};
#endif
