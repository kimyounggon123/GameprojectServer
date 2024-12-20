#ifndef _PACKETFORBROADCAST_H
#define _PACKETFORBROADCAST_H


#include <queue>
#include "Packet.h"
#include "PacketControllerclass.h"
class PacketForBroadcast
{
	std::queue<Packet*>broadcast_queue;

	HANDLE broad_event;
	void change_header(Packet& pk);

public:
	PacketForBroadcast(){
		broad_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	~PacketForBroadcast() {
		CloseHandle(broad_event);
	}

	ERROR_CODE add_pk(Packet* pk);
	ERROR_CODE add_pk(const Packet& pk);
	ERROR_CODE pop_packet(Packet* pk);
};
#endif