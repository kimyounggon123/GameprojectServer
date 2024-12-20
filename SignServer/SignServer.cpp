#include "SignServer.h"
Key key;

VOID SignServer::initialize() {
	IOCPclass::initialize();

	key.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	key.hMutex = CreateMutex(NULL, FALSE, NULL);

	sign_proc.initialize(); // to use function
}
SignServer::~SignServer() {
	CloseHandle(key.hEvent);
	CloseHandle(key.hMutex);
}

void SignServer::renew_data_length(SOCKETINFO* ptr, DWORD cbTransferred) {
	ErrorMsgclass& emsg = server_info->singletons->err_msg;
	PacketControllerclass& pk_control = server_info->singletons->pk_control; // to use deserialize()
	if (ptr == nullptr) {
		emsg.error_print(ERROR_CODE::GET_NULLPTR);
		return;
	}

	ERROR_CODE err_code; // get error code

	ptr->recvbytes = cbTransferred;
	ptr->sendbytes = 0;
	ptr->buf[ptr->recvbytes] = '\0'; // close buffer

	Packet* pk = new Packet; // make a packet
	err_code = pk_control.deserialize(ptr->buf, *pk);		// deserialize
	if (err_code != ERROR_CODE::COMPLETE_THIS) {			// Can't deserialize
		emsg.error_print(err_code, "SignServer renew data");
		return;
	}


	// Packet Data Structures
	PK_TYPE type = static_cast<PK_TYPE>(pk->header.type);
	auto func = sign_proc.get_func(type);		// bring the function that match the header
	err_code = func(pk, ptr);

	
	if (err_code != ERROR_CODE::COMPLETE_THIS)
	{ // get out this method if you get a undefined function
		emsg.error_print(err_code, "SignServer renew data 2\n");
		return;
	}
	std::cout << "data: " << pk->data << std::endl;
	
	packet_queue.add_pkto_db(pk);		// add a packet in queue for interaction with DB

}

void SignServer::send_data(SOCKETINFO* ptr) {
	ErrorMsgclass& emsg = server_info->singletons->err_msg;
	//printf("서버가 보낸 데이터 내용: %s\n", ptr->buf);
	
	// Pop queue
	Packet* send_this_to_client = packet_queue.bring_pkto_client(); // bring a packet from queue
	if (send_this_to_client == nullptr)
	{
		emsg.error_print(ERROR_CODE::GET_NULLPTR, "[SignServer send_data]");
		return;
	}

	
	SOCKET_AND_ADDR who_will_get;
	PK_TYPE type = static_cast<PK_TYPE>(send_this_to_client->header.type);
	auto func = sign_proc.get_func_fromDB(type);
	ERROR_CODE err_code = func(send_this_to_client, &who_will_get);
	if (err_code != ERROR_CODE::COMPLETE_THIS)
	{
		emsg.error_print(err_code, "SignServer fucntion process");
		return;
	}

	/// serialize
	PacketControllerclass& pk_control = server_info->singletons->pk_control; // to use serialize()
	char buffer[BUFFERSIZE + 1];
	err_code = pk_control.serialize(buffer, *send_this_to_client);

	if (err_code != ERROR_CODE::COMPLETE_THIS)
	{
		emsg.error_print(err_code, "SignServer serialize");
		return;
	}
	/// \serialize

	// Real send part
	ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
	ptr->wsabuf.buf = buffer;
	ptr->wsabuf.len = send_this_to_client->header.length + sizeof(PacketHeader);

	INT retval;
	DWORD sendbytes;


	retval = WSASend(who_will_get.sock, &ptr->wsabuf, 1, &sendbytes,
		0, &ptr->overlapped, NULL);
	if (retval == SOCKET_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING)
			emsg.err_display(_T("send_to_clients"));
	}

	// memory free
	delete send_this_to_client;
	
	//send_data_echo(ptr);
}