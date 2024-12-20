#include "ServerRunner.h"
ServerRunner* ServerRunner::instance = nullptr;

void ServerRunner::initialize() {
	ErrorMsgclass& emsg = ErrorMsgclass::getInstance();
	hThreads[0] = (HANDLE)_beginthreadex(
		NULL, 0,
		server_run, this,
		0, (unsigned*)&dwThreadID[0]
	);
	if (hThreads[0] == NULL) emsg.err_quit(_T("[ServerRunner] _beginthreadex()"));
	hThreads[1] = (HANDLE)_beginthreadex(
		NULL, 0,
		DBconnector_run, this,
		0, (unsigned*)&dwThreadID[1]
	);
	if (hThreads[1] == NULL) emsg.err_quit(_T("[ServerRunner] _beginthreadex()"));

	server->initialize();
	DBconnector->initialize();
}
VOID ServerRunner::run() {
	WaitForMultipleObjects(RUNNER_THREADS, hThreads, TRUE, INFINITE);
}

unsigned int WINAPI ServerRunner::server_run(LPVOID lpParam) {
	ServerRunner* This = static_cast<ServerRunner*>(lpParam);
	This->server->run();
	return 0;
}
unsigned int WINAPI ServerRunner::DBconnector_run(LPVOID lpParam) {
	ServerRunner* This = static_cast<ServerRunner*>(lpParam);
	This->DBconnector->run();
	return 0;
}