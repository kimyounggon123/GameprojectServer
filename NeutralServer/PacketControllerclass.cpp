#include "PacketControllerclass.h"
#include <WinSock2.h>
PacketControllerclass* PacketControllerclass::instance = nullptr;

ERROR_CODE PacketControllerclass::serialize(char* buffer, const Packet& pk) {
	if (buffer == nullptr) return ERROR_CODE::GET_NULLPTR;
	if (pk.header.type == static_cast<uint16_t>(PK_TYPE::BE_DELETED)) return ERROR_CODE::HAVETO_DELETE_PACKET_HEADER;
	uint16_t type = pk.header.type;
	uint16_t length = pk.header.length; // Add end mark size

	if (length > BUFFERSIZE)
		return ERROR_CODE::EXCEED_SIZE;

	// copy header field to buffer
	memcpy(buffer, &type, sizeof(type));
	memcpy(buffer + sizeof(type), &length, sizeof(length));

	// copy data part
	memcpy(buffer + sizeof(PacketHeader), pk.data, length);

	// add end mark
	//USHORT end_mark = END_MARK;
	//memcpy(buffer + sizeof(PacketHeader) + length, &end_mark, sizeof(end_mark));

	// close buffer
	buffer[sizeof(type) + sizeof(length) + length] = '\0';

	return ERROR_CODE::COMPLETE_THIS;
}

// Make Packet out of this class.
ERROR_CODE PacketControllerclass::deserialize(const char* client_buffer, Packet& pk) {
	if (client_buffer == nullptr) return ERROR_CODE::GET_NULLPTR;

	memset(&pk, 0, sizeof(Packet));
	// copy type and length data
	memcpy(&pk.header.type, client_buffer, sizeof(pk.header.type));
	memcpy(&pk.header.length, client_buffer + sizeof(pk.header.type), sizeof(pk.header.length));

	//pk.header.type = (pk.header.type);
	//pk.header.length = (pk.header.length);

	// copy data part
	memcpy(pk.data, client_buffer + sizeof(PacketHeader), pk.header.length + 1);

	return ERROR_CODE::COMPLETE_THIS;
}

ERROR_CODE PacketControllerclass::set_packet(Packet* pk, PK_TYPE type, const char* to_copy) {
	if (pk == nullptr || to_copy == nullptr) return ERROR_CODE::GET_NULLPTR;

	size_t length = strlen(to_copy); // use strlen only once
	if (length + sizeof(uint16_t) > BUFFERSIZE)
		return ERROR_CODE::EXCEED_SIZE;

	pk->header.type = static_cast<uint16_t>(type);

	size_t offset = 0;
	// 1. copy to_copy length
	memcpy(pk->data, &length, sizeof(uint16_t));
	offset += sizeof(uint16_t);
	// 2. copy to_copy 
	memcpy(pk->data + offset, to_copy, length); // Copy data part
	offset += length;

	pk->header.length = static_cast<uint32_t>(offset);

	pk->data[offset] = '\0'; // close buffer
	return ERROR_CODE::COMPLETE_THIS;
}

ERROR_CODE PacketControllerclass::add_data_in_packet(Packet* pk, const char* added) {
	if (pk == nullptr || added == nullptr) return ERROR_CODE::GET_NULLPTR;
	if (pk->header.type == static_cast<uint16_t>(PK_TYPE::BE_DELETED)) return ERROR_CODE::HAVETO_DELETE_PACKET_HEADER;

	// size comparing
	size_t current_len = pk->header.length;
	size_t extra_len = strlen(added);
	if (current_len + static_cast<uint16_t>(extra_len + sizeof(uint16_t)) > BUFFERSIZE)
		return ERROR_CODE::EXCEED_SIZE;

	// Copy part
	size_t offset = current_len;
	// 1. copy added length
	memcpy(pk->data + offset, &extra_len, sizeof(uint16_t));
	offset += sizeof(uint16_t);
	// 2. copy added 
	memcpy(pk->data + offset, added, extra_len); // Copy data part
	offset += extra_len;

	// reset length
	pk->header.length = static_cast<uint16_t>(offset);
	pk->data[pk->header.length] = '\0';

	return ERROR_CODE::COMPLETE_THIS;
}
