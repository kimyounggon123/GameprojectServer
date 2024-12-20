#include "ProcessClass.h"
ProcessClass* ProcessClass::instance = nullptr;

ERROR_CODE ProcessClass::chat(Packet* pk) {
	if (pk == nullptr) return ERROR_CODE::GET_NULLPTR;
	ERROR_CODE err_code;

	// chat typing part
	char chat_buffer[BUFFERSIZE + 1];
	std::cout << "[Chat]: ";
	std::cin.getline(chat_buffer, BUFFERSIZE); // 동기적으로 입력 대기

	try {
		// check command 
		if (_stricmp("/game", chat_buffer) == 0) {

			err_code = pcc.set_packet(pk, PK_TYPE::GO_TO_GAMEROOM, chat_buffer);
			if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code;
		}
		if (_stricmp("/center", chat_buffer) == 0) {

			err_code = pcc.set_packet(pk, PK_TYPE::GO_TO_CENTER, chat_buffer);
			if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code;
		}
		if (_stricmp("/quit", chat_buffer) == 0) {

			err_code = pcc.set_packet(pk, PK_TYPE::LEAVE_TO_SERVER, chat_buffer);
			if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code;
		}

		// if it is not command, it just chat
		err_code = pcc.set_packet(pk, PK_TYPE::SEND_MSG, chat_buffer);
		if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code;
	}
	catch (ERROR_CODE err) {
		return err;
	}

	return err_code;
}

ERROR_CODE ProcessClass::move(Packet* pk) {
	printf("테스트 용 move\n");
	if (pk == nullptr) return ERROR_CODE::GET_NULLPTR;

	return ERROR_CODE::COMPLETE_THIS;
}