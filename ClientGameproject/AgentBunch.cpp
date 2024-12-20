#include "AgentBunch.h"
AgentBunch* AgentBunch::instance = nullptr;


AgentBunch::AgentBunch(): chat_agent(new ChatAgent), proc_agent(new ProcessAgent){
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
}

AgentBunch::~AgentBunch() {

	if (chat_agent)
	{
		delete chat_agent;
		chat_agent = nullptr;
	}
	if (proc_agent)
	{
		delete proc_agent;
		proc_agent = nullptr;
	}
	WSACleanup(); // close winsock
}