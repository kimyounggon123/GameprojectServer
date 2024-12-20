#include <tchar.h>
#include "DBprocessServer.h"
int _tmain(int argc, TCHAR* argv[]) {

	DBprocessServer* server = new DBprocessServer;
	server->initialize();
	server->run();

	return 0;
}