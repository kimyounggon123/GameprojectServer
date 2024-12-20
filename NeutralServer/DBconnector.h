#ifndef _DBCONNECTOR_H
#define _DBCONNECTOR_H

#include <WinSock2.h>
#include <Windows.h>
#include <process.h> // _beginthreadex
#pragma comment(lib,"ws2_32")

#include "Singletons.h"
#include "stdafx.h"
#include "SOCKETINFO.h"


#define THREADS 2
class DBconnector
{
protected:
	bool exit_flag;
	Singletons* singleton;

	WSADATA wsa;
	SOCKET_AND_ADDR connector;

	HANDLE hThreads[THREADS];
	DWORD dwThreadID[THREADS];

	char recv_buf[BUFFERSIZE + 1];
	char send_buf[BUFFERSIZE + 1];

	virtual INT send_toDB() = 0;
	virtual INT recv_fromDB() = 0;

	DBconnector();
public:
	virtual ~DBconnector();

	virtual void initialize();
	VOID run();

	static unsigned int WINAPI send_db_thread(LPVOID lpParam);
	static unsigned int WINAPI recv_db_thread(LPVOID lpParam);
};
#endif


