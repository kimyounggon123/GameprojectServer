#include "GameroomServer.h"
void GameroomServer::initialize() {
	IOCPclass::initialize();
	process.initialize();
}

void GameroomServer::packet_process(SOCKETINFO* ptr) {
	if (ptr == nullptr) return;
	PacketControllerclass& pcc = server_info->singletons->pk_control;
	ErrorMsgclass& emsg = server_info->singletons->err_msg;

	Packet pk;
	ERROR_CODE err = pcc.deserialize(ptr->buf, pk);
	if (err != ERROR_CODE::COMPLETE_THIS) {
		emsg.error_print(err, "packet_process deserialize");
		return;
	}
	PK_TYPE type = static_cast<PK_TYPE>(pk.header.type);
	auto func = process.get_func(type);
	if (!func) {
		emsg.err_display(_T("Non defined packet header!\n"));
		return;
	}
	err = func(&pk, ptr, *cm);
	if (err != ERROR_CODE::COMPLETE_THIS) {
		emsg.error_print(err, "packet_process func");
	}
}
void GameroomServer::renew_data_length(SOCKETINFO* ptr, DWORD cbTransferred) {
	IOCPclass::renew_data_length(ptr, cbTransferred);
	packet_process(ptr); // pk process
}
void GameroomServer::send_data(SOCKETINFO* ptr) {
	Singletons& singleton = *server_info->singletons;
	ErrorMsgclass& emsg = singleton.err_msg;

	ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
	ptr->wsabuf.buf = ptr->buf;
	ptr->wsabuf.len = ptr->recvbytes;

	INT retval;
	DWORD sendbytes;

	retval = WSASend(ptr->sa.sock, &ptr->wsabuf, 1, &sendbytes,
		0, &ptr->overlapped, NULL);
	if (retval == SOCKET_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING)
			emsg.err_display(_T("send_to_clients"));
	}
}

void GameroomServer::broadcast_for_all() {
	Singletons& singleton = *server_info->singletons;
	ErrorMsgclass& emsg = singleton.err_msg;
	PacketControllerclass& pcc = singleton.pk_control;
	std::vector<Client*>clients = cm->get_all_clients();

	Packet pk;
	ERROR_CODE err = broadcaster.pop_packet(&pk);
	if (err != ERROR_CODE::COMPLETE_THIS) {
		emsg.err_display(_T("Failed to prepare broadcast data."));
		return;
	}
	int send_len = pk.header.length + sizeof(PacketHeader); // get Packet length


	// serialize
	INT retval;
	char* pk_buf = new char[send_len + 1];


	try {
		err = pcc.serialize(pk_buf, pk);
		if (err != ERROR_CODE::COMPLETE_THIS) {
			emsg.err_display(_T("serialize fail."));
			throw err;
		}
		for (auto client : clients) {
			SOCKET client_sock = client->get_sock();
			if (client_sock == INVALID_SOCKET) {
				emsg.err_display(_T("Invalid socket for broadcast."));
				continue;
			}

			// 클라이언트별로 WSABUF 및 OVERLAPPED 생성
			WSABUF* wsabuf = new WSABUF; // 동적 할당
			wsabuf->buf = pk_buf;
			wsabuf->len = send_len;
			OVERLAPPED* overlapped = new OVERLAPPED; // 비동기 작업을 위한 구조체
			ZeroMemory(overlapped, sizeof(OVERLAPPED));

			DWORD sendbytes = 0;
			retval = WSASend(client->get_sock(), wsabuf, 1, &sendbytes,
				0, overlapped, NULL);
			if (retval == SOCKET_ERROR) {
				if (WSAGetLastError() != WSA_IO_PENDING) {
					emsg.err_display(_T("send_to_clients"));
					delete wsabuf; wsabuf = nullptr;
					delete overlapped; overlapped = nullptr;
					throw ERROR_CODE::FUNCTION_RUNNING_FAILED;
				}
			}
			delete wsabuf; wsabuf = nullptr;
			delete overlapped; overlapped = nullptr;
		}
	}
	catch (ERROR_CODE err) {
		emsg.error_print(err);
	}

	// memory free
	delete[] pk_buf; pk_buf = nullptr;
}