#ifndef _IOCPCLASS_H
#define _IOCPCLASS_H

#include "stdafx.h"
#include "ServerInfo.h" // SOCKETINFO, Serverinfo

#include "ClientManager.h"
#include <process.h> // for using _beginthreadex()

class IOCPclass
{
protected:
	ServerInfo* server_info; // it has SOCKET, SOCKADDR_IN, Utils instance
	bool exit_flag;

	HANDLE IOCP;

	ClientManager* cm;
	/// <IOCP thread process (overlapped process)>
	virtual void renew_data_length(SOCKETINFO* ptr, DWORD cbTransferred);
	virtual void send_data(SOCKETINFO* ptr);

	void get_data(SOCKETINFO* ptr);
	void send_data_echo(SOCKETINFO* ptr); // echo 

	virtual void broadcast_for_all();
	/// </IOCP thread process (overlapped process)>

public:
	IOCPclass(USHORT port_num);
	virtual ~IOCPclass();

	virtual VOID initialize();	// put threads in IOCP 
	VOID run();

	static unsigned int WINAPI workerThread(LPVOID server_info);
};
#endif