#include "IOCPclass.h"

IOCPclass::IOCPclass(USHORT port_num):IOCP(NULL), exit_flag(false), cm(new ClientManager){
	server_info = new ServerInfo;
	if (WSAStartup(MAKEWORD(2, 2), &server_info->WSAdata) != 0) return;
	server_info->SERVERPORT = port_num;
}
IOCPclass::~IOCPclass() {
	WSACleanup(); // close winsock
	delete cm; cm = nullptr;
	delete server_info; server_info = nullptr;
}
VOID IOCPclass::initialize() {
	ErrorMsgclass& err_msg = server_info->singletons->err_msg;
	SOCKET_AND_ADDR& server = server_info->server;
	try {
		// Making socket
		server.sock = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
		if (server.sock == INVALID_SOCKET) throw _T("WSASocket()");

		INT retval;
		BOOL optval = TRUE;
		retval = setsockopt(server.sock, SOL_SOCKET,
			SO_REUSEADDR, (char*)&optval, sizeof(BOOL));
		if (retval == SOCKET_ERROR) throw _T("setsockopt()");

		// bind()
		ZeroMemory(&server.addr, sizeof(SOCKADDR_IN));
		server.addr.sin_family = AF_INET;
		server.addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		server.addr.sin_port = htons(server_info->SERVERPORT);
		retval = bind(server.sock, (SOCKADDR*)&server.addr, sizeof(SOCKADDR_IN));
		if (retval == SOCKET_ERROR) throw _T("bind()");

		// listen()
		retval = listen(server.sock, SOMAXCONN);
		if (retval == SOCKET_ERROR) throw _T("listen()");

		// initialize IOCP
		IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, NULL);

		// Get num of CPU
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		HANDLE hThread; // make threads to put in IOCP
		for (size_t i = 0; i < static_cast<size_t>(si.dwNumberOfProcessors * 2); i++) {
			hThread = (HANDLE)_beginthreadex(
				NULL, 0,
				workerThread, this,
				0, NULL);
			if (hThread == NULL) err_msg.err_quit(_T("_beginthreadex()"));
			CloseHandle(hThread); // if you put thread, close it (it is exist in IOCP)
		}
	}
	catch (LPCTSTR Msg) {
		err_msg.err_quit(Msg);
	}
}
VOID IOCPclass::run() {
	INT retval;
	DWORD flags, recvbytes;
	flags = 0;
	int addrlen = sizeof(SOCKADDR_IN);

	Singletons& singletons = *server_info->singletons;

	SOCKET_AND_ADDR& server = server_info->server;
	SOCKET_AND_ADDR& client = server_info->client;
	ErrorMsgclass& err_msg = singletons.err_msg;
	try {
		// get memories of socket info 
		// server
		SOCKET& listen_sock = server.sock;
		// client
		SOCKET& client_sock = client.sock;
		SOCKADDR_IN& client_addr = client.addr;

		while (!exit_flag) {
			client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addrlen);
			if (client_sock == INVALID_SOCKET) throw _T("accept()");

			_tprintf(_T("Someone comes to here!\n"));

			// Connect client socket at IOCP
			CreateIoCompletionPort((HANDLE)client_sock, IOCP, client_sock, 0);

			// make a new client socket info
			SOCKETINFO* new_ptr = new SOCKETINFO;
			if (new_ptr == NULL) throw _T("SOCKETINFO pointer is NULL");
			ZeroMemory(&new_ptr->overlapped, sizeof(new_ptr->overlapped));
			new_ptr->sa.sock = client_sock;
			new_ptr->recvbytes = new_ptr->sendbytes = 0;
			new_ptr->wsabuf.buf = new_ptr->buf;
			new_ptr->wsabuf.len = BUFFERSIZE;
			new_ptr->sa.addr = client_addr;

			// Start IO
			retval = WSARecv(client_sock, &new_ptr->wsabuf, 1, &recvbytes,
				&flags, &new_ptr->overlapped, NULL);

			if (retval == SOCKET_ERROR) {
				if (WSAGetLastError() != ERROR_IO_PENDING) {
					delete new_ptr;
					new_ptr = nullptr;
					err_msg.err_display(_T("WSARecv()"));
				}
				continue;
			}
		}
	}
	catch (LPCTSTR Msg) {
		err_msg.err_quit(Msg);
	}

}
unsigned int WINAPI IOCPclass::workerThread(LPVOID server_info) {
	IOCPclass* This = reinterpret_cast<IOCPclass*>(server_info);
	ErrorMsgclass& emsg = This->server_info->singletons->err_msg;

	HANDLE IOCP = This->IOCP;
	INT retval;
	while (!This->exit_flag) {
		DWORD cbTransferred;
		SOCKET client_sock; // get this at GetQueuedCompletionStatus
		SOCKETINFO* ptr;

		// Get IO result
		retval = GetQueuedCompletionStatus(IOCP, &cbTransferred,
			(PULONG_PTR)&client_sock, (LPOVERLAPPED*)&ptr, INFINITE);
		
		// get client information
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(SOCKADDR_IN);
		getpeername(ptr->sa.sock, (SOCKADDR*)&clientaddr, &addrlen);
		// if client is leaved
		if (retval == 0 || cbTransferred == 0) {
			if (retval == 0) {
				DWORD temp1, temp2;
				WSAGetOverlappedResult(ptr->sa.sock, &ptr->overlapped,
					&temp1, FALSE, &temp2);
				emsg.err_display(_T("WSAGetOverlappedResult()"));
			}
			closesocket(ptr->sa.sock);
			delete ptr;
			ptr = nullptr;
			continue;
		}

		/// <socketinfo process>
		if (ptr->recvbytes == 0) This->renew_data_length(ptr, cbTransferred);
		else ptr->sendbytes += cbTransferred;

		if (ptr->recvbytes > ptr->sendbytes) This->send_data(ptr);
		else This->get_data(ptr);
		/// </socketinfo process>
		This->broadcast_for_all();
	}
	return 0;
}

void IOCPclass::renew_data_length(SOCKETINFO* ptr, DWORD cbTransferred) {
	if (ptr == nullptr) return;
	// renew data length
	ptr->recvbytes = cbTransferred;
	ptr->sendbytes = 0;
	ptr->buf[ptr->recvbytes] = '\0'; // close buffer
}
void IOCPclass::send_data(SOCKETINFO* ptr) {
	if (ptr == nullptr) return;
	send_data_echo(ptr);
}
void IOCPclass::send_data_echo(SOCKETINFO* ptr) { // echo system
	if (ptr == nullptr) return;
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

void IOCPclass::get_data(SOCKETINFO* ptr) {
	if (ptr == nullptr) return;
	Singletons& singleton = *server_info->singletons;
	PacketControllerclass& pcc = singleton.pk_control;
	ErrorMsgclass& emsg = singleton.err_msg;

	ptr->recvbytes = 0;
	INT retval;

	// get data from clients
	ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
	ptr->wsabuf.buf = ptr->buf;
	ptr->wsabuf.len = BUFFERSIZE;

	DWORD recvbytes;
	DWORD flags = 0;
	retval = WSARecv(ptr->sa.sock, &ptr->wsabuf, 1,
		&recvbytes, &flags, &ptr->overlapped, NULL);
	if (retval == SOCKET_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			emsg.err_display(_T("WSARecv()"));
		}
	}
}

void IOCPclass::broadcast_for_all()
{

}