#ifndef _SERVERINFO_H
#define _SERVERINFO_H

#include "SOCKETINFO.h"
#include "Singletons.h" // it has packet controller, clientlist, roomclass

typedef struct _Key {
	HANDLE hMutex;
	HANDLE hEvent;
}Key;
typedef struct _SERVERINFO {
	// server info
	USHORT SERVERPORT;
	WSADATA WSAdata;

	SOCKET_AND_ADDR server;
	SOCKET_AND_ADDR client;

	Singletons* singletons;

	_SERVERINFO() :
		singletons(new Singletons), SERVERPORT(0),
		WSAdata({}), server(INVALID_SOCKET, {0}), client(INVALID_SOCKET, { 0 })
	{}
	~_SERVERINFO()
	{
		delete singletons;
		singletons = nullptr;
	}
}ServerInfo;
#endif