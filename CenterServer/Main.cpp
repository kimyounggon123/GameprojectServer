#include <tchar.h>
#include "CenterServer.h"
int _tmain(int argc, TCHAR* argv[]) {

	CenterServer* server = new CenterServer;

	server->initialize();
	server->run();

	return 0;
}