#include "stdafx.h"
#include "ClientAgent.h"

int _tmain() {

	ClientAgent* agent = new ClientAgent;
	agent->initialize();
	agent->run();

	return 0;
}