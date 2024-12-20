#ifndef _SERVERRUNNER_H
#define _SERVERRUNNER_H

#include "SignDBconnector.h"
#include "SignServer.h"

#define RUNNER_THREADS 2
class ServerRunner
{
	SignDBconnector* DBconnector;
	SignServer* server;


	HANDLE hThreads[RUNNER_THREADS];
	DWORD dwThreadID[RUNNER_THREADS];

	static ServerRunner* instance;
	ServerRunner() :
		DBconnector(new SignDBconnector), server(new SignServer)
	{
		for (int i = 0; i < RUNNER_THREADS; i++) {
			hThreads[i] = NULL;
			dwThreadID[i] = 0;
		}
	}
public:
	static ServerRunner& getInstance() {
		if (instance == nullptr) instance = new ServerRunner;
		return *instance;
	}
	~ServerRunner() {
		delete server; delete DBconnector;
		server = nullptr; DBconnector = nullptr;
		for (int i = 0; i < RUNNER_THREADS; i++) {
			CloseHandle(hThreads[i]);
			dwThreadID[i] = 0;
		}
	}

	void initialize();
	VOID run();

	static unsigned int WINAPI server_run(LPVOID lpParam);
	static unsigned int WINAPI DBconnector_run(LPVOID lpParam);
};
#endif

