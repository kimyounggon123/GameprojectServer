#ifndef _SOCKETINFO_H
#define _SOCKETINFO_H

#include "stdafx.h"
#include <WinSock2.h>
#include <MSWSock.h>
#include <Windows.h>

#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "ws2_32.lib")
typedef struct _SOCKETDUO {
	SOCKET sock;
	SOCKADDR_IN addr; // to compare clients

	_SOCKETDUO():
		sock(INVALID_SOCKET)
	{
		ZeroMemory(&addr, sizeof(addr));
	}

	_SOCKETDUO(SOCKET sock, SOCKADDR_IN addr) :
		sock(sock), addr(addr)
	{}

	_SOCKETDUO(const _SOCKETDUO& other)
	{
		memcpy(&sock, &other.sock, sizeof(SOCKET));
		memcpy(&addr, &other.addr, sizeof(SOCKADDR_IN));
	}

} SOCKET_AND_ADDR;

typedef struct _SOCKETINFO {
	OVERLAPPED overlapped;
	SOCKET_AND_ADDR sa;

	INT address_len;

	char buf[BUFFERSIZE + 1];
	int recvbytes;
	int sendbytes;

	WSABUF wsabuf;

	_SOCKETINFO() 
	{}

	~_SOCKETINFO() {
		closesocket(sa.sock);
	}
	bool operator==(const _SOCKETINFO& other_addr) {
		if (this->sa.addr.sin_port != other_addr.sa.addr.sin_port ||
			this->sa.addr.sin_addr.s_addr != other_addr.sa.addr.sin_addr.s_addr) return false;
		return true;
	}
} SOCKETINFO;

#endif 

