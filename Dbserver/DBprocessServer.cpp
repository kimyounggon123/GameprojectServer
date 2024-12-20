#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "DBprocessServer.h"
Key key;

DBprocessServer::DBprocessServer() 
	:IOCPclass(BASIC_DB_PORT), procs(PKprocsBunch::getInstance()), QUEUE(DBPKqueue::getInstance())
{
	IOCPclass::initialize();

	key.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	key.hMutex = CreateMutex(NULL, FALSE, NULL);

	procs.initialize(); // to use function
}
DBprocessServer::~DBprocessServer() {
	CloseHandle(key.hEvent);
	CloseHandle(key.hMutex);
}

VOID DBprocessServer::initialize() {
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
		server.addr.sin_addr.S_un.S_addr = inet_addr(BASIC_DB_SERVER_IP);
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

void DBprocessServer::renew_data_length(SOCKETINFO* ptr, DWORD cbTransferred) {
	if (ptr == nullptr) return;
	Singletons& singleton = *server_info->singletons;
	ErrorMsgclass& emsg = singleton.err_msg;
	PacketControllerclass& pcc = singleton.pk_control;

	//printf("hewqeqweqw");
	// renew data length
	ptr->recvbytes = cbTransferred;
	ptr->sendbytes = 0;
	ptr->buf[ptr->recvbytes] = '\0'; // close buffer 여기서 문제 발생.

	ERROR_CODE err_code;
	Packet* db_pk = new Packet;

	try {
		err_code = pcc.deserialize(ptr->buf, *db_pk);
		if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code;
		PK_TYPE type = static_cast<PK_TYPE>(db_pk->header.type);
		
		/*printf("Server to DB Packet Data (Length: %u): ", db_pk->header.length);
		for (size_t i = 0; i < db_pk->header.length; ++i) {
			printf("%02X ", db_pk->data[i]);
		}
		printf("\n");*/

		err_code = procs.do_proc(type, db_pk); // it also changes header 여기 문제 있음.
		if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code;
	}

	catch (ERROR_CODE code) {
		emsg.error_print(code);
	}
	
	QUEUE.add_pkto_clients(db_pk);
}

void DBprocessServer::send_data(SOCKETINFO* ptr) {
	if (ptr == nullptr) return;
	ErrorMsgclass& emsg = server_info->singletons->err_msg;

	Packet* pk = QUEUE.bring_pkto_client();
	if (pk == nullptr)
	{
		emsg.error_print(ERROR_CODE::GET_NULLPTR, "[SignServer send_data]");
		return;
	}

	/// Real send part
	PacketControllerclass& pk_control = server_info->singletons->pk_control; // to use serialize()
	char buffer[BUFFERSIZE + 1];
	ERROR_CODE err_code = pk_control.serialize(buffer, *pk);
	if (err_code != ERROR_CODE::COMPLETE_THIS)
	{
		emsg.error_print(err_code, "SignServer serialize");
		return;
	}
	/*
	printf("클라이언트에게 보낼 데이터 Data (Length: %u): ", pk->header.length);
	for (size_t i = 0; i < pk->header.length; ++i) {
		printf("%02X ", pk->data[i]);
	}
	*/
	// Real send part
	ZeroMemory(&ptr->overlapped, sizeof(ptr->overlapped));
	ptr->wsabuf.buf = buffer;
	ptr->wsabuf.len = pk->header.length + sizeof(PacketHeader);

	INT retval;
	DWORD sendbytes;

	retval = WSASend(ptr->sa.sock, &ptr->wsabuf, 1, &sendbytes,
		0, &ptr->overlapped, NULL);
	if (retval == SOCKET_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING)
			emsg.err_display(_T("send_to_clients"));
	}
	// memory free
	delete pk;


	//send_data_echo(ptr);
}