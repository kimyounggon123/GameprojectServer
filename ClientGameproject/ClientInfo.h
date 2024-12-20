#ifndef _CLIENTINFO_H
#define _CLIENTINFO_H
#include <Windows.h>

// server
#define SIGN_SERVERPORT 6000
#define CHAT_SERVER_PORT 5000
#define SERVERIP   "127.0.0.1"

#define THREAD_NUM 2
typedef struct _THREAD {

	HANDLE hThreads[THREAD_NUM];
	DWORD dwThreadID[THREAD_NUM];

}THREAD_INFO;

typedef struct _TOCONNECTSERVER {
	// winsock and socket setting
	SOCKET sock;
	SOCKADDR_IN server_address;

}TOCONNECTSERVER;


typedef struct _gKey {
	HANDLE hEvent;
	HANDLE hMutex;
}gKey;
#endif
