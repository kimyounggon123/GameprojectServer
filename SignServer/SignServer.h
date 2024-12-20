#ifndef _SIGNSERVER_H
#define _SIGNSERVER_H

#include "IOCPclass.h"
#include "Packet.h"

#include "DBPKqueue.h"
#include "PacketSignclass.h"

class SignServer
	:public IOCPclass
{
	DBPKqueue& packet_queue;
	PacketSignclass& sign_proc;

	void renew_data_length(SOCKETINFO* ptr, DWORD cbTransferred) override;
	void send_data(SOCKETINFO* ptr) override;

public:
	SignServer() :IOCPclass(SIGN_SERVERPORT),
		packet_queue(DBPKqueue::getInstance()), sign_proc(PacketSignclass::getInstance())
	{}
	~SignServer();

	VOID initialize() override;
};

#endif