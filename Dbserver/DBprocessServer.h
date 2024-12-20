#ifndef _DBPROCESSSERVER_H
#define _DBPROCESSSERVER_H


#include "IOCPclass.h"
#include "PKprocsBunch.h"
#include "DBPKqueue.h"

class DBprocessServer
	:public IOCPclass
{

	PKprocsBunch& procs;
	DBPKqueue& QUEUE;
	void renew_data_length(SOCKETINFO* ptr, DWORD cbTransferred) override;
	void send_data(SOCKETINFO* ptr) override;

public:
	DBprocessServer();
	~DBprocessServer();

	VOID initialize() override;
};

#endif

