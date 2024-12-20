#ifndef _PACKET_H
#define _PACKET_H

#define _CRT_SECURE_NO_WARNINGS
#include <cstdint> // int_t
#include <cstring>

#include "SOCKETINFO.h"
// server <-> clients
enum class PK_TYPE : uint16_t {
	BE_DELETED = 0,		// default packet

	// sign up
	TRY_SIGN_UP,		// header / (ID / Password)
	SIGN_UP_COMPLETE,	// header
	SIGN_UP_FAIL,		// header

	// log in / sign in
	TRY_SIGN_IN,		// header / (ID / Password)
	SIGN_IN_COMPLETE,	// header / CENTER_PORT_NUM
	SIGN_IN_FAIL,		// header

	// client->server
	COME_TO_SERVER,		// header / ID
	LEAVE_TO_SERVER,	// header / ID
	SEND_MSG,			// header / (ID / msg)
	MY_MOVE,			// header / (ID / pos)

	// server -> client
	BROADCAST_WELCOME,	// header / ID
	BROADCAST_BYE,		// header / ID
	SOMEONES_MSG,		// header / (ID / msg)
	SOMEONE_MOVED,		// header / (ID / pos)

	GO_TO_GAMEROOM,		// (client -> server) header / ID
	SEND_GAMEROOM_PORT,	// (server -> client) header / GAMEROOM_PORT_NUM

	GO_TO_CENTER,		// (client -> server) header / ID
	SEND_CENTER_PORT	// (server -> client) header / CENTER_PORT_NUM
};	

#define END_MARK 0xff

#pragma pack(push, 1)
typedef struct _PacketHeader
{
	uint16_t type;		// packet type
	uint16_t length;	// packet length, it contains only data length.

}PacketHeader;

typedef struct _Packet
{
	PacketHeader header;
	char data[BUFFERSIZE + 1];

	_Packet()
	{
		header.type = static_cast<uint16_t>(PK_TYPE::BE_DELETED);
		header.length = 0;
		memset(this->data, 0, sizeof(this->data));
	}

	_Packet(PK_TYPE type, const char* data)
	{
		if (data == nullptr || type == PK_TYPE::BE_DELETED) {
			header.type = static_cast<uint16_t>(PK_TYPE::BE_DELETED);
			header.length = 0;
			memset(this->data, 0, sizeof(this->data));
		}
		
		else {
			size_t length = strlen(data); // calc data length
			this->header.type = static_cast<uint16_t>(type); // input header type in packet
			this->header.length = static_cast<uint16_t>(length); // input data length in packet
			memcpy(this->data, data, length + 1);  // input data in packet
		}
	}

	_Packet(const _Packet& other)
	{
		memcpy(&header, &other.header, sizeof(PacketHeader));
		strcpy_s(data, other.data);
		uint16_t len = static_cast<uint16_t>(strlen(data));
		data[len] = '\0';
		header.length = len;
	}


}Packet;



#pragma pack(pop)
#endif
