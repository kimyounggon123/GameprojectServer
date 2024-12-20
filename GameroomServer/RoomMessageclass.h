#ifndef _ROOMMESSAGECLASS_H
#define _ROOMMESSAGECLASS_H

#include "Packet.h"
#include "ClientManager.h"
#include "PacketMessageclass.h"
class RoomMessageclass
	:public PacketMessageclass
	
{
	static RoomMessageclass* instance;
	RoomMessageclass():PacketMessageclass(){}

public:
	static RoomMessageclass& getInstance() {
		if (instance == nullptr) instance = new RoomMessageclass;
		return *instance;
	}

	/// <run these in PacketProcessclass>
	void welcome_client(const char* ID, const SOCKETINFO& info);
	void bye_client(const char* ID, const SOCKETINFO& info);
	void leave_gameroom(const char* ID, const SOCKETINFO& info);
	void someone_chat(const char* ID, const char* data, const SOCKETINFO& info);
	void someone_move(const char* ID, const char* data, const SOCKETINFO& info);
	/// </run these in PacketProcessclass>
};
#endif