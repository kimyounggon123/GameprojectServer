#include "PacketForBroadcast.h"

void PacketForBroadcast::change_header(Packet& pk) {
	PK_TYPE change_this = static_cast<PK_TYPE>(pk.header.type); // default type
	//std::cout << "original header: " << static_cast<int>(change_this) << std::endl;
	switch (change_this) {
	case PK_TYPE::COME_TO_SERVER:
		change_this = PK_TYPE::BROADCAST_WELCOME;
		break;
	case PK_TYPE::LEAVE_TO_SERVER:
		change_this = PK_TYPE::BROADCAST_BYE;
		break;
	case PK_TYPE::SEND_MSG:
		change_this = PK_TYPE::SOMEONES_MSG;
		break;
	case PK_TYPE::MY_MOVE:
		change_this = PK_TYPE::SOMEONE_MOVED;
		break;
	default:
		// handle unexpected cases or leave as it is
		break;
	}
	//std::cout << "original header: " << static_cast<int>(change_this) << std::endl;
	pk.header.type = static_cast<uint16_t>(change_this);

}
ERROR_CODE PacketForBroadcast::add_pk(Packet* pk) {
	if (pk == nullptr) return ERROR_CODE::GET_NULLPTR;

	change_header(*pk); // change headers
	broadcast_queue.push(pk);

	SetEvent(broad_event);
	return ERROR_CODE::COMPLETE_THIS;
}
ERROR_CODE PacketForBroadcast::add_pk(const Packet& pk) {
	Packet* new_pk = new Packet;
	if (new_pk == nullptr) return ERROR_CODE::GET_NULLPTR;
	memcpy(new_pk, &pk, sizeof(Packet));

	broadcast_queue.push(new_pk); 

	SetEvent(broad_event);
	return ERROR_CODE::COMPLETE_THIS;
}
ERROR_CODE PacketForBroadcast:: pop_packet(Packet* pk){
	WaitForSingleObject(broad_event, INFINITE);
	if (pk != nullptr) 
		return ERROR_CODE::GET_NULLPTR;
	

	PacketControllerclass& ppc = PacketControllerclass::getInstance();
	pk = broadcast_queue.front();
	broadcast_queue.pop();
	if (pk == nullptr) return ERROR_CODE::GET_NULLPTR;
	if (broadcast_queue.empty()) ResetEvent(broad_event);

	return ERROR_CODE::COMPLETE_THIS;
}
