#ifndef _PACKETCENTERCLASS_H
#define _PACKETCENTERCLASS_H

#include "stdafx.h"
#include "PacketProcessclass.h"
#include "CenterMessageclass.h"
class PacketCenterclass
	:public PacketProcessclass
{
	CenterMessageclass& pmsg;

	std::map<PK_TYPE, std::function<ERROR_CODE(Packet*, SOCKETINFO*)>> process;
	std::function<ERROR_CODE(Packet*, SOCKETINFO*)> welcome;
	std::function<ERROR_CODE(Packet*, SOCKETINFO*)> bye;
	std::function<ERROR_CODE(Packet*, SOCKETINFO*)> chat;
	std::function<ERROR_CODE(Packet*, SOCKETINFO*)> goto_gameroom;

	static PacketCenterclass* instance;
	PacketCenterclass():PacketProcessclass(), pmsg(CenterMessageclass::getInstance()){}
public:
	static PacketCenterclass& getInstance() {
		if (instance == nullptr) instance = new PacketCenterclass;
		return *instance;
	}

	void input_gameroom_port(Packet& pk);
	void initialize() override;
	std::function<ERROR_CODE(Packet*, SOCKETINFO*)> get_func(PK_TYPE type) { return process[type]; }
};
#endif
