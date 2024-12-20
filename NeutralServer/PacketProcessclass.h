#ifndef _PACKETPROCESSCLASS_H
#define _PACKETPROCESSCLASS_H

#include "stdafx.h"
#include "ClientManager.h"

#include "Packet.h"
#include "SOCKETINFO.h"
#include "PacketForBroadcast.h"

#include <functional>
#include <map>
#include <ctime>

typedef struct _Time_values {
	uint16_t year;   // 20xx
	uint8_t month;   // 1 ~ 12
	uint8_t day;     // 1 ~ 31
	uint8_t hour;    // 0 ~ 23
	uint8_t minute;  // 0 ~ 59
	uint8_t second;  // 0 ~ 59

	bool input_time_values();
} TIME_IN_PACKET;

class PacketProcessclass
{
protected:
	ERROR_CODE input_time_info(Packet& pk);
public:
	PacketProcessclass(){}
	virtual ~PacketProcessclass(){}

	ERROR_CODE set_data_to_PORTNUM(Packet* pk, USHORT port_num);
	ERROR_CODE copy_string_in_packet(Packet* pk, char*& string_copy, size_t& offset);
	virtual void initialize() = 0;
};


#endif
