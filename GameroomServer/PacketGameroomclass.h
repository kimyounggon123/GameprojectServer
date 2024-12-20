#ifndef _PACKETGAMEROOMCLASS_H
#define _PACKETGAMEROOMCLASS_H

#include "PacketProcessclass.h"
#include "RoomMessageclass.h"
#include "GameroomBroadcast.h"
class PacketGameroomclass
	:public PacketProcessclass
{
	RoomMessageclass& pmsg;
	GameroomBroadcast& pbroad;

	std::map<PK_TYPE, std::function<ERROR_CODE(Packet*, SOCKETINFO*, ClientManager&)>> process; // client -> server process
	std::function<ERROR_CODE(Packet*, SOCKETINFO*, ClientManager&)> welcome;
	std::function<ERROR_CODE(Packet*, SOCKETINFO*, ClientManager&)> bye;
	std::function<ERROR_CODE(Packet*, SOCKETINFO*, ClientManager&)> goto_center;
	std::function<ERROR_CODE(Packet*, SOCKETINFO*, ClientManager&)> msg;
	std::function<ERROR_CODE(Packet*, SOCKETINFO*, ClientManager&)> move;


	ERROR_CODE welcome_func(Packet*, SOCKETINFO*, ClientManager&);
	ERROR_CODE bye_func(Packet*, SOCKETINFO*, ClientManager&) ;
	ERROR_CODE goto_center_func(Packet*, SOCKETINFO*, ClientManager&);
	ERROR_CODE msg_func(Packet*, SOCKETINFO*, ClientManager&);
	ERROR_CODE move_func(Packet*, SOCKETINFO*, ClientManager&);


	static PacketGameroomclass* instance;
	PacketGameroomclass() : PacketProcessclass(),
		pmsg(RoomMessageclass::getInstance()) , pbroad(GameroomBroadcast::getInstance()){}
public:
	static PacketGameroomclass& getInstance() {
		if (instance == nullptr) instance = new PacketGameroomclass;
		return *instance;
	}

	~PacketGameroomclass() {}
	void initialize() override;

	std::function<ERROR_CODE(Packet*, SOCKETINFO*, ClientManager& cm)> get_func(PK_TYPE type) {
		auto func = process[type];
		if (!func) return [](Packet* pk, SOCKETINFO* info, ClientManager& cm) {
			return ERROR_CODE::UNDEFINED_FUNCTION;
			};
		return func;
	}
};
#endif
