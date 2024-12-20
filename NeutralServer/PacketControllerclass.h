#ifndef _PACKETCONTROLLERCLASS_H
#define _PACKETCONTROLLERCLASS_H


#include <vector>
#include "Packet.h"


////////////////////////////////////////////////////////////////////////////
// Please make a packet out of this class!!!!
////////////////////////////////////////////////////////////////////////////
class PacketControllerclass
{
	static PacketControllerclass* instance;
	PacketControllerclass() {}
public:
	// PacketControllerclass(){}
	static PacketControllerclass& getInstance() {
		if (instance == nullptr) instance = new PacketControllerclass;
		return *instance;
	}


	ERROR_CODE serialize(char* buffer, const Packet& pk);
	ERROR_CODE deserialize(const char* client_buffer, Packet& pk);

	ERROR_CODE set_packet(Packet* pk, PK_TYPE type, const char* to_copy);
	ERROR_CODE add_data_in_packet(Packet* pk, const char* added);

};

#endif