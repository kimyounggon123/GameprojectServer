#include "PacketGameroomclass.h"

PacketGameroomclass* PacketGameroomclass::instance = nullptr;
ERROR_CODE PacketGameroomclass::welcome_func(Packet* pk, SOCKETINFO* info, ClientManager& cm)  {
	ERROR_CODE code = ERROR_CODE::COMPLETE_THIS;
	char* ID = nullptr; size_t offset = 0;
	try {
		ERROR_CODE err = copy_string_in_packet(pk, ID, offset);
		if (err != ERROR_CODE::COMPLETE_THIS) return err;
		Client* client = new Client(pk->data, info);
		pmsg.welcome_client(pk->data, *info);
		pbroad.add_pk(pk);
	}
	catch (ERROR_CODE err) {
		code = err;
	}

	if (ID)
	{
		delete[] ID; ID = nullptr;
	}
	return code;
}
ERROR_CODE PacketGameroomclass::bye_func(Packet* pk, SOCKETINFO* info, ClientManager& cm) {
	if (pk == nullptr || info == nullptr) return ERROR_CODE::GET_NULLPTR;
	ERROR_CODE code = ERROR_CODE::COMPLETE_THIS;
	char* ID = nullptr; size_t offset = 0;

	try {
		ERROR_CODE err = copy_string_in_packet(pk, ID, offset);
		if (err != ERROR_CODE::COMPLETE_THIS) return err;

		Client* client = cm.find_client(ID);
		cm.del_client(client);
		pmsg.bye_client(ID, *info);

		pbroad.add_pk(pk);
	}
	catch (ERROR_CODE err) {
		code = err;
	}

	if (ID)
	{
		delete[] ID; ID = nullptr;
	}
	return code;
}
ERROR_CODE PacketGameroomclass::goto_center_func(Packet* pk, SOCKETINFO* info, ClientManager& cm) {

	if (pk == nullptr || info == nullptr) return ERROR_CODE::GET_NULLPTR;
	ERROR_CODE code = ERROR_CODE::COMPLETE_THIS;
	char* ID = nullptr; size_t offset = 0;

	try {
		ERROR_CODE err = copy_string_in_packet(pk, ID, offset);
		if (err != ERROR_CODE::COMPLETE_THIS) return err;

		Client* client = cm.find_client(ID);
		cm.del_client(client);
		pmsg.leave_gameroom(ID, *info);

		pbroad.add_pk(pk);
	}
	catch (ERROR_CODE err) {
		code = err;
	}

	if (ID)
	{
		delete[] ID; ID = nullptr;
	}
	return code;
}
ERROR_CODE PacketGameroomclass::msg_func(Packet* pk, SOCKETINFO* info, ClientManager& cm) {
	if (pk == nullptr || info == nullptr) return ERROR_CODE::GET_NULLPTR;
	ERROR_CODE code = ERROR_CODE::COMPLETE_THIS;
	char* ID = nullptr; char* data = nullptr; size_t offset = 0;

	try {
		ERROR_CODE err = copy_string_in_packet(pk, ID, offset);
		if (err != ERROR_CODE::COMPLETE_THIS) return err;
		err = copy_string_in_packet(pk, data, offset);
		if (err != ERROR_CODE::COMPLETE_THIS) return err;

		pmsg.someone_chat(ID, data, *info);

		pbroad.add_pk(pk);
	}
	catch (ERROR_CODE err) {
		code = err;
	}

	if (ID)
	{
		delete[] ID; ID = nullptr;
	}
	if (data)
	{
		delete[] data; data = nullptr;
	}
	return code;
}
ERROR_CODE PacketGameroomclass::move_func(Packet* pk, SOCKETINFO* info, ClientManager& cm) {
	if (pk == nullptr || info == nullptr) return ERROR_CODE::GET_NULLPTR;
	ERROR_CODE code = ERROR_CODE::COMPLETE_THIS;
	char* ID = nullptr; char* data = nullptr; size_t offset = 0;
	try {
		ERROR_CODE err = copy_string_in_packet(pk, ID, offset);
		if (err != ERROR_CODE::COMPLETE_THIS) return err;
		err = copy_string_in_packet(pk, data, offset);
		if (err != ERROR_CODE::COMPLETE_THIS) return err;

		pmsg.someone_chat(ID, data, *info);

		pbroad.add_pk(pk);
	}
	catch (ERROR_CODE err) {
		code = err;
	}

	if (ID)
	{
		delete[] ID; ID = nullptr;
	}
	if (data)
	{
		delete[] data; data = nullptr;
	}
	return code;
}
void PacketGameroomclass::initialize() {

	welcome = [this](Packet* pk, SOCKETINFO* info, ClientManager& cm) {
		return welcome_func(pk, info, cm);
		};

	bye = [this](Packet* pk, SOCKETINFO* info, ClientManager& cm) {
		return bye_func(pk, info, cm);
		};

	goto_center = [this](Packet* pk, SOCKETINFO* info, ClientManager& cm) {
		return goto_center_func(pk, info, cm);
		};

	msg = [this](Packet* pk, SOCKETINFO* info, ClientManager& cm) {
		return msg_func(pk, info, cm);
		};

	move = [this](Packet* pk, SOCKETINFO* info, ClientManager& cm) {
		return move_func(pk, info, cm);
		};


	process[PK_TYPE::COME_TO_SERVER] = welcome;
	process[PK_TYPE::LEAVE_TO_SERVER] = bye;
	process[PK_TYPE::GO_TO_CENTER] = goto_center;
	process[PK_TYPE::SEND_MSG] = msg;
	process[PK_TYPE::MY_MOVE] = move;
}