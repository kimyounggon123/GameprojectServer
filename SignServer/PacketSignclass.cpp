#include "PacketSignclass.h"
PacketSignclass* PacketSignclass::instance = nullptr;

ERROR_CODE PacketSignclass::sign_in_try_func(Packet* pk, SOCKETINFO* info) {
	if (pk == nullptr || info == nullptr) return ERROR_CODE::GET_NULLPTR;

	char* ID = nullptr; char* PW = nullptr;
	size_t offset = 0;
	
	try {
		ERROR_CODE err_code = copy_string_in_packet(pk, ID, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;
		err_code = copy_string_in_packet(pk, PW, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;

		// input client's socket info
		size_t socket_length = sizeof(info->sa);
		memcpy(pk->data + offset, &info->sa, socket_length);
		pk->header.length += socket_length;
		pk->data[pk->header.length] = '\0';

		sign_msg.try_signin(info->sa, ID, PW); // print msg
	}
	catch (ERROR_CODE err) {
		if (ID)
		{
			delete[] ID;
			ID = nullptr;
		}
		if (PW)
		{
			delete[] PW;
			PW = nullptr;
		}
		return err;
	}

	delete[] ID;
	ID = nullptr;
	delete[] PW;
	PW = nullptr;

	return ERROR_CODE::COMPLETE_THIS;
}
ERROR_CODE PacketSignclass::sign_up_try_func(Packet* pk, SOCKETINFO* info) {
	if (pk == nullptr || info == nullptr) return ERROR_CODE::GET_NULLPTR;
	char* ID = nullptr; char* PW = nullptr;
	size_t offset = 0;

	try {
		ERROR_CODE err_code = copy_string_in_packet(pk, ID, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;
		err_code = copy_string_in_packet(pk, PW, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;
		
		// input client's socket info
		size_t socket_length = sizeof(info->sa);
		memcpy(pk->data + offset, &info->sa, socket_length);
		pk->header.length += socket_length;
		pk->data[pk->header.length] = '\0';

		sign_msg.try_signup(info->sa, ID, PW); // print msg
	}
	catch (ERROR_CODE err) {
		if (ID)
		{
			delete[] ID;
			ID = nullptr;
		}
		if (PW)
		{
			delete[] PW;
			PW = nullptr;
		}
		return err;
	}

	delete[] ID;
	ID = nullptr;
	delete[] PW;
	PW = nullptr;
	return ERROR_CODE::COMPLETE_THIS;
}

ERROR_CODE PacketSignclass::sign_in_success_func(Packet* pk, SOCKET_AND_ADDR* client) {
	if (pk == nullptr || client == nullptr) return ERROR_CODE::GET_NULLPTR;

	char* ID = nullptr; char* PW = nullptr;


	SOCKET_AND_ADDR who_sign;
	size_t offset = 0;
	try {
		ERROR_CODE err_code = copy_string_in_packet(pk, ID, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;

		err_code = copy_string_in_packet(pk, PW, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;

		// you should copy client socket info
		if (offset > pk->header.length) throw ERROR_CODE::EXCEED_SIZE;

		// copy socket info
		memcpy(client, pk->data + offset, sizeof(SOCKET_AND_ADDR));

		sign_msg.signin_done(*client, ID, PW); // print msg

		err_code = set_data_to_PORTNUM(pk, CENTER_PORT);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;
	}
	catch (ERROR_CODE err) {
		if (ID)
		{
			delete[] ID;
			ID = nullptr;
		}
		if (PW)
		{
			delete[] PW;
			PW = nullptr;
		}
		return err;
	}

	delete[] ID;
	ID = nullptr;
	delete[] PW;
	PW = nullptr;
	return ERROR_CODE::COMPLETE_THIS;
}
ERROR_CODE PacketSignclass::sign_up_success_func(Packet* pk, SOCKET_AND_ADDR* client) {
	if (pk == nullptr || client == nullptr) return ERROR_CODE::GET_NULLPTR;

	char* ID = nullptr; char* PW = nullptr;
	char* client_sockinfo = nullptr;
	size_t offset = 0;

	try {
		ERROR_CODE err_code = copy_string_in_packet(pk, ID, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;

		err_code = copy_string_in_packet(pk, PW, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;

		// you should copy client socket info
		if (offset > pk->header.length) {
			throw ERROR_CODE::EXCEED_SIZE;
		}

		memcpy(client, pk->data + offset, sizeof(SOCKET_AND_ADDR));

		sign_msg.signup_done(*client, ID, PW); // print msg
	}
	catch (ERROR_CODE err) {
		if (ID)
		{
			delete[] ID;
			ID = nullptr;
		}
		if (PW)
		{
			delete[] PW;
			PW = nullptr;
		}
		return err;
	}

	delete[] ID;
	ID = nullptr;
	delete[] PW;
	PW = nullptr;
	return ERROR_CODE::COMPLETE_THIS;
}

ERROR_CODE PacketSignclass::sign_in_fail_func(Packet* pk, SOCKET_AND_ADDR* client) {
	if (pk == nullptr || client == nullptr) return ERROR_CODE::GET_NULLPTR;

	char* ID = nullptr; char* PW = nullptr;
	char* client_sockinfo = nullptr;
	size_t offset = 0;

	try {
		ERROR_CODE err_code = copy_string_in_packet(pk, ID, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;

		err_code = copy_string_in_packet(pk, PW, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;

		// you should copy client socket info
		if (offset > pk->header.length)
			throw ERROR_CODE::EXCEED_SIZE;


		memcpy(client, pk->data + offset, sizeof(SOCKET_AND_ADDR));

		sign_msg.signin_fail(*client, ID, PW); // print msg
	}
	catch (ERROR_CODE err) {
		if (ID)
		{
			delete[] ID;
			ID = nullptr;
		}
		if (PW)
		{
			delete[] PW;
			PW = nullptr;
		}
		return err;
	}

	delete[] ID;
	ID = nullptr;
	delete[] PW;
	PW = nullptr;
	return ERROR_CODE::COMPLETE_THIS;
}
ERROR_CODE PacketSignclass::sign_up_fail_func(Packet* pk, SOCKET_AND_ADDR* client) {
	if (pk == nullptr || client == nullptr) return ERROR_CODE::GET_NULLPTR;

	char* ID = nullptr; char* PW = nullptr;
	char* client_sockinfo = nullptr;
	size_t offset = 0;

	try {
		ERROR_CODE err_code = copy_string_in_packet(pk, ID, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;

		err_code = copy_string_in_packet(pk, PW, offset);
		if (err_code != ERROR_CODE::COMPLETE_THIS)
			throw err_code;

		// you should copy client socket info
		if (offset  > pk->header.length)
			throw ERROR_CODE::EXCEED_SIZE;

		memcpy(client, pk->data + offset, sizeof(SOCKET_AND_ADDR));

		sign_msg.signup_fail(*client, ID, PW); // print msg
	}
	catch (ERROR_CODE err) {
		if (ID)
		{
			delete[] ID;
			ID = nullptr;
		}
		if (PW)
		{
			delete[] PW;
			PW = nullptr;
		}
		return err;
	}

	delete[] ID;
	ID = nullptr;
	delete[] PW;
	PW = nullptr;
	return ERROR_CODE::COMPLETE_THIS;
}

void PacketSignclass::initialize() {
	/// packet process from client
	sign_up_try = [this](Packet* pk, SOCKETINFO* info) {
		return sign_up_try_func(pk, info);
		};
	sign_in_try = [this](Packet* pk, SOCKETINFO* info) {
		return sign_in_try_func(pk, info);
		};

	/// packet process from DB
	sign_up_complete = [this](Packet* pk, SOCKET_AND_ADDR* sa) {
		return sign_up_success_func(pk, sa);
		};

	sign_in_complete = [this](Packet* pk, SOCKET_AND_ADDR* sa) {
		return sign_in_success_func(pk, sa);
		};

	sign_up_fail = [this](Packet* pk, SOCKET_AND_ADDR* sa) {
		return sign_up_fail_func(pk, sa);
		};

	sign_in_fail = [this](Packet* pk, SOCKET_AND_ADDR* sa) {
		return sign_in_fail_func(pk, sa);
		};


	process[PK_TYPE::TRY_SIGN_UP] = sign_up_try;
	process[PK_TYPE::TRY_SIGN_IN] = sign_in_try;

	process_fromDB[PK_TYPE::SIGN_UP_COMPLETE] = sign_up_complete;
	process_fromDB[PK_TYPE::SIGN_IN_COMPLETE] = sign_in_complete;
	process_fromDB[PK_TYPE::SIGN_UP_FAIL] = sign_up_fail;
	process_fromDB[PK_TYPE::SIGN_IN_FAIL] = sign_in_fail;

}


std::function<ERROR_CODE(Packet*, SOCKETINFO*)> PacketSignclass::get_func(PK_TYPE type) {
	auto func = process[type];
	if (!func) return [this](Packet* pk, SOCKETINFO* info) {
		if (pk == nullptr || info == nullptr) return ERROR_CODE::GET_NULLPTR;
		return ERROR_CODE::UNDEFINED_FUNCTION;
		};
	return func;
}
std::function<ERROR_CODE(Packet*, SOCKET_AND_ADDR*)> PacketSignclass::get_func_fromDB(PK_TYPE type) {
	auto func = process_fromDB[type];
	if (!func) return [this](Packet* pk, SOCKET_AND_ADDR* info) {
		if (pk == nullptr || info == nullptr) return ERROR_CODE::GET_NULLPTR;
		return ERROR_CODE::UNDEFINED_FUNCTION;
		};;
	return func;
}