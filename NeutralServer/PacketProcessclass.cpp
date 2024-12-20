#include "PacketProcessclass.h"


bool _Time_values::input_time_values() {
	time_t timer = time(NULL);
	struct tm t;
	if (localtime_s(&t, &timer) != 0) return false;
	this->year = t.tm_year + 1900;
	this->month = t.tm_mon + 1;     
	this->day = t.tm_mday;
	this->hour = t.tm_hour;
	this->minute = t.tm_min;
	this->second = t.tm_sec;

	return true;
}
ERROR_CODE PacketProcessclass::input_time_info(Packet& pk) {
	TIME_IN_PACKET tip;
	if (!tip.input_time_values()) return ERROR_CODE::FUNCTION_RUNNING_FAILED;
	
	if (sizeof(pk.data) < sizeof(TIME_IN_PACKET)) return ERROR_CODE::EXCEED_SIZE;
	memcpy(pk.data, &tip, sizeof(TIME_IN_PACKET));
	pk.data[sizeof(TIME_IN_PACKET)] = '\0';		 // closing buffer
	pk.header.length = sizeof(PacketHeader);	// set length
	pk.header.type = static_cast<uint16_t>(PK_TYPE::BROADCAST_WELCOME);

	return ERROR_CODE::COMPLETE_THIS;
}
ERROR_CODE PacketProcessclass::copy_string_in_packet(Packet* pk, char*& string_copy, size_t& offset) {
	if (pk == nullptr) return ERROR_CODE::GET_NULLPTR;
	uint16_t str_len = 0;
	
	// 1. Copy string length
	memcpy(&str_len, pk->data + offset, sizeof(uint16_t));
	offset += sizeof(uint16_t);
	//std::cout << "해당 동적 데이터 길이: " << str_len << std::endl;


	// 2. Check string length validity
	if (str_len == 0 || str_len > BUFFERSIZE) {
		return ERROR_CODE::EXCEED_SIZE;
	}

	// 3. Allocate memory for string
	string_copy = new char[str_len + 1];
	if (string_copy == nullptr) {
		return ERROR_CODE::MEMORY_LIMIT;
	}

	// 4. Copy string data
	memcpy(string_copy, pk->data + offset, str_len);
	string_copy[str_len] = '\0'; // Null-terminate the string
	offset += str_len;


	//std::cout << "결과: " << string_copy << std::endl;
	return ERROR_CODE::COMPLETE_THIS;
}

ERROR_CODE PacketProcessclass::set_data_to_PORTNUM(Packet* pk, USHORT port_num) {
	/// <Process for sending packet to client>
	memset(pk->data, 0, BUFFERSIZE + 1); // memory set(don't send other data)
	// add PORT NUM
	size_t offset = sizeof(USHORT);
	memcpy(pk->data, &port_num, offset);
	pk->header.length = static_cast<uint16_t>(offset);
	pk->data[offset] = '\0';
	///

	return ERROR_CODE::COMPLETE_THIS;
}