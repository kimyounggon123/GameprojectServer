#include "stdafx.h"
#include "ServerRunner.h"
int _tmain(int argc, TCHAR* argv[]) {
	ServerRunner& runner = ServerRunner::getInstance();

	runner.initialize();
	runner.run();

	return 0;
}