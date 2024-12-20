#include "SignDBproc.h"

ERROR_CODE SignDBproc::client_sign_in_proc(Packet* pk) {
	if (pk == nullptr) return ERROR_CODE::GET_NULLPTR;

	char* ID = nullptr;
	char* PW = nullptr;
	SOCKET_AND_ADDR client_socket_info;
	size_t offset = 0;
	/*
	printf("헤더 값: %d\n", static_cast<PK_TYPE>(pk->header.type));
	printf("처리하려는 패킷 (Length: %u): ", pk->header.length);
	for (size_t i = 0; i < pk->header.length; ++i) {
		printf("%02X ", pk->data[i]);
	}
	printf("\n");
	*/
	try {
		ERROR_CODE err_code = copy_string_in_packet(pk, ID, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code;
		err_code = copy_string_in_packet(pk, PW, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code;

		if (offset + sizeof(SOCKET_AND_ADDR) > pk->header.length) throw ERROR_CODE::EXCEED_SIZE;
		memcpy(&client_socket_info, pk->data + offset, sizeof(SOCKET_AND_ADDR));

		//// SQL part
		QUERY_RESULT query_r = manager->check_user_exists(ID, PW); // get query result
		if (query_r != QUERY_RESULT::COMPLETE) {			// if query result is error (sign in error)
			pk->header.type = static_cast<uint16_t>(PK_TYPE::SIGN_IN_FAIL);
			std::cout << "오류 코드: " << static_cast<uint16_t>(query_r) << std::endl;
			printf("SQL fail\n");
		}
		else {
			pk->header.type = static_cast<uint16_t>(PK_TYPE::SIGN_IN_COMPLETE);
			printf("SQL success\n");
		}

		
		query_r = manager->write_sign_in_log(ID, client_socket_info); // get query result
		if (query_r != QUERY_RESULT::COMPLETE) {			// if query result is error (sign in error)
			pk->header.type = static_cast<uint16_t>(PK_TYPE::SIGN_IN_FAIL);
			printf("SQL fail\n");
		}
		else {
			pk->header.type = static_cast<uint16_t>(PK_TYPE::SIGN_IN_COMPLETE);
			printf("SQL success\n");
		}
		
		////
	}
	catch (ERROR_CODE err) {

		if (ID)
		{
			delete[] ID; ID = nullptr;
		}
		if (PW)
		{
			delete[] PW; PW = nullptr;
		}
		return err;
	}
	delete[] ID; ID = nullptr;
	delete[] PW; PW = nullptr;
	return ERROR_CODE::COMPLETE_THIS;
}
ERROR_CODE SignDBproc::client_sign_up_proc(Packet* pk) {
	if (pk == nullptr) return ERROR_CODE::GET_NULLPTR;

	char* ID = nullptr;
	char* PW = nullptr;
	size_t offset = 0;
	
	SOCKET_AND_ADDR client_sockinfo;
	try {
		ERROR_CODE err_code = copy_string_in_packet(pk, ID, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code;
		err_code = copy_string_in_packet(pk, PW, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code;
		
		if (offset + sizeof(SOCKET_AND_ADDR) > pk->header.length) throw ERROR_CODE::EXCEED_SIZE;
		memcpy(&client_sockinfo, pk->data + offset, sizeof(SOCKET_AND_ADDR));

		//// SQL part
		QUERY_RESULT query_r = manager->add_new_user(ID, PW); // get query result
		if (query_r != QUERY_RESULT::COMPLETE) {			// if query result is error (sign in error)
			pk->header.type = static_cast<uint16_t>(PK_TYPE::SIGN_UP_FAIL);
			printf("SQL fail\n");
		}
		else {
			pk->header.type = static_cast<uint16_t>(PK_TYPE::SIGN_UP_COMPLETE);
			printf("SQL success\n");
		}
		////
	}
	catch (ERROR_CODE err) {

		if (ID)
		{
			delete[] ID; ID = nullptr;
		}
		if (PW)
		{
			delete[] PW; PW = nullptr;
		}
		return err;
	}
	delete[] ID; ID = nullptr;
	delete[] PW; PW = nullptr;
	return ERROR_CODE::COMPLETE_THIS;
}

void SignDBproc::initialize() {
	if (!manager) return;

	sign_in_proc = [this](Packet* pk_db) {
		return client_sign_in_proc(pk_db);
		};

	sign_up_proc = [this](Packet* pk_db) {
		return client_sign_up_proc(pk_db);
		};


	proc[PK_TYPE::TRY_SIGN_IN] = sign_in_proc;
	proc[PK_TYPE::TRY_SIGN_UP] = sign_up_proc;

	manager->initialize();
}

std::function<ERROR_CODE(Packet*)> SignDBproc::get_func(PK_TYPE type) {
	auto func = proc[type];
	if (!func) return [this](Packet* pk) {
		if (pk == nullptr) return ERROR_CODE::GET_NULLPTR;
		return ERROR_CODE::UNDEFINED_FUNCTION;
		};
	return func;
}