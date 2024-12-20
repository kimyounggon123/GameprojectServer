#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "DBconnector.h"

DBconnector::DBconnector()
	:exit_flag(false), singleton(new Singletons)
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
	for (int i = 0; i < THREADS; i++) {
		hThreads[i] = NULL;
		dwThreadID[i] = 0;
	}
	ZeroMemory(recv_buf, sizeof(recv_buf));
	ZeroMemory(send_buf, sizeof(send_buf));
}
DBconnector::~DBconnector() {
	closesocket(connector.sock);
	delete singleton; singleton = nullptr;
}

void DBconnector::initialize() {
	ErrorMsgclass& emsg = singleton->err_msg;

	// SOCKET
	connector.sock = socket(AF_INET, SOCK_STREAM, 0);
	if (connector.sock == INVALID_SOCKET) emsg.err_quit(_T("socket() IOCP"));

	hThreads[0] = (HANDLE)_beginthreadex(
		NULL, 0,
		send_db_thread, this,
		0, (unsigned*)&dwThreadID[0]
	);
	if (hThreads[0] == NULL) emsg.err_quit(_T("[DBconnector] _beginthreadex()"));
	hThreads[1] = (HANDLE)_beginthreadex(
		NULL, 0,
		recv_db_thread, this,
		0, (unsigned*)&dwThreadID[1]
	);
	if (hThreads[1] == NULL) emsg.err_quit(_T("[DBconnector] _beginthreadex()"));
}

VOID DBconnector::run() {
	WaitForMultipleObjects(THREADS, hThreads, TRUE, INFINITE);
}

unsigned int WINAPI DBconnector::send_db_thread(LPVOID lpParam) {
	DBconnector* This = static_cast<DBconnector*>(lpParam);
	ErrorMsgclass& emsg = This->singleton->err_msg;

	SOCKET_AND_ADDR& connector = This->connector;

	INT retval;

	while (!This->exit_flag) {
		retval = This->send_toDB();
		if (retval == SOCKET_ERROR) emsg.err_display(_T("send_toDB()"));
	}

	return 0;
}
unsigned int WINAPI DBconnector::recv_db_thread(LPVOID lpParam) {
	DBconnector* This = static_cast<DBconnector*>(lpParam);
	ErrorMsgclass& emsg = This->singleton->err_msg;

	SOCKET_AND_ADDR& connector = This->connector;

	INT retval;

	while (!This->exit_flag) {
		retval = This->recv_fromDB();
		if (retval == SOCKET_ERROR) emsg.err_display(_T("send_toDB()"));
	}

	return 0;
}