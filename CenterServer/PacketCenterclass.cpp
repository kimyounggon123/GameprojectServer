#include "PacketCenterclass.h"
PacketCenterclass* PacketCenterclass::instance = nullptr;

void PacketCenterclass::input_gameroom_port(Packet& pk) {
	ZeroMemory(pk.data, sizeof(pk.data));
	pk.header.type = static_cast<uint16_t>(PK_TYPE::SEND_GAMEROOM_PORT);
	USHORT for_input = GAMEROOM_PORT;
	memcpy(pk.data, &for_input, sizeof(USHORT));
}

void PacketCenterclass::initialize() {
	welcome = [this](Packet* pk, SOCKETINFO* info) {
		ERROR_CODE err = ERROR_CODE::COMPLETE_THIS;
		if (pk == nullptr || info == nullptr) return ERROR_CODE::GET_NULLPTR;
		char* ID = nullptr;
		try {
			size_t offset = 0;
			ERROR_CODE err = copy_string_in_packet(pk, ID, offset);
			if (err != ERROR_CODE::COMPLETE_THIS) throw err;

			pmsg.welcome_client(ID, *info);
		}
		catch (ERROR_CODE code) {
			err = code;
		}

		if (ID) {
			delete ID; 
			ID = nullptr;
		}

		return err;
		};
	bye = [this](Packet* pk, SOCKETINFO* info) {
		if (pk == nullptr || info == nullptr) return ERROR_CODE::GET_NULLPTR;
		char* ID = nullptr;
		ERROR_CODE err = ERROR_CODE::COMPLETE_THIS;
		try {
			size_t offset = 0;
			ERROR_CODE err = copy_string_in_packet(pk, ID, offset);
			if (err != ERROR_CODE::COMPLETE_THIS) throw err;

			pmsg.bye_client(ID, *info);
		}
		catch (ERROR_CODE code) {
			err = code;
		}

		if (ID) {
			delete ID;
			ID = nullptr;
		}
		return err;
		};
	chat = [this](Packet* pk, SOCKETINFO* info) {
		if (pk == nullptr || info == nullptr) return ERROR_CODE::GET_NULLPTR;
		char* ID = nullptr; char* chat = nullptr;
		ERROR_CODE err = ERROR_CODE::COMPLETE_THIS;
		try {
			size_t offset = 0;
			ERROR_CODE err = copy_string_in_packet(pk, chat, offset);
			if (err != ERROR_CODE::COMPLETE_THIS) throw err;
			err = copy_string_in_packet(pk, ID, offset);
			if (err != ERROR_CODE::COMPLETE_THIS) throw err;

			pmsg.client_msg(ID, chat, *info);
		}
		catch(ERROR_CODE code){
			err = code;
		}
		if (ID) {
			delete ID;
			ID = nullptr;
		}
		if (chat) {
			delete chat;
			chat = nullptr;
		}
		return err;
		};
	goto_gameroom = [this](Packet* pk, SOCKETINFO* info) {
		if (pk == nullptr || info == nullptr) return ERROR_CODE::GET_NULLPTR;
		char* ID = nullptr;
		ERROR_CODE err = ERROR_CODE::COMPLETE_THIS;
		try {
			size_t offset = 0;
			ERROR_CODE err = copy_string_in_packet(pk, ID, offset);
			if (err != ERROR_CODE::COMPLETE_THIS) throw err;
			pmsg.some_goto_gameroom(ID, *info);

			input_gameroom_port(*pk);
		}
		catch (ERROR_CODE code) {
			err = code;
		}
		if (ID) {
			delete ID;
			ID = nullptr;
		}
		return err;
		};
	process[PK_TYPE::COME_TO_SERVER] = welcome;
	process[PK_TYPE::LEAVE_TO_SERVER] = bye;
	process[PK_TYPE::SEND_MSG] = chat;
	process[PK_TYPE::GO_TO_GAMEROOM] = goto_gameroom;
}