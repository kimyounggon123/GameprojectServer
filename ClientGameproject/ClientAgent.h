#ifndef _CLIENTAGENT_H
#define _CLIENTAGENT_H

#include <WinSock2.h>
#include <Windows.h>
#include <process.h> // _beginthreadex

#include "stdafx.h"
#include "HandlerGroup.h"
#include "InputKeyboard.h"

#pragma comment(lib,"ws2_32")

#include "ClientInfo.h" // key TOCONNECTSERVER, some macros
#include "ProcessClass.h" // chat and move methods in here

#include "SignClass.h" // sign process
class ClientAgent
{
protected:
	WSADATA wsa;

	TOCONNECTSERVER connector; // it has socket info 

	THREAD_INFO thread_info; // thread info
	HandlerGroup* handler; // error msg, packet controll, etc.

	ProcessClass& proc; // keybord input process
	/// <extra attributes>
	bool exit_flag;
	/// </extra attributes>

	bool log_in_process();
	ERROR_CODE process_packet_from_server(const char* got_pk, Packet& packet);

	bool reset_portinfo(USHORT port_num);

	HANDLE send_event;
	HANDLE recv_event;
	// your info
	SignClass& sign_info;


public:
	ClientAgent();
	~ClientAgent();


	virtual void initialize();
	virtual VOID run();


	/// <threads>
	static unsigned int WINAPI update_thread(LPVOID lpParam); // frame update thread
	static unsigned int WINAPI process_thread(LPVOID lpParam); // chatting thread
	/// </threads>

	/// <process methods>
	ERROR_CODE process_result(Packet* pk, int ch);
	/// </process methods>
};
#endif
