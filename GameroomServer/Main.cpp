#include <tchar.h>

#include "GameroomServer.h"
int _tmain(int argc, TCHAR* argv[]) {
	GameroomServer* server = new GameroomServer(1001);

	server->initialize();
	server->run();

	return 0;
}