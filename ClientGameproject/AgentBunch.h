#ifndef _AGENTBUNCH_H
#define _AGENTBUNCH_H
#include <WinSock2.h>
#include <Windows.h>
#include <process.h> // _beginthreadex

#include "stdafx.h"
#include "ChatAgent.h"
#include "ProcessAgent.h"
class AgentBunch
{

	WSADATA wsa;

	ChatAgent* chat_agent;
	ProcessAgent* proc_agent;

	static AgentBunch* instance;
	AgentBunch();

public:
	static AgentBunch& getInstance() {
		if (instance == nullptr) instance = new AgentBunch;
		return *instance;
	}
	~AgentBunch();


	VOID run();

	//static unsigned int WINAPI chat_thread(LPVOID lpParam);
	//static unsigned int WINAPI proc_thread(LPVOID lpParam);
};

#endif