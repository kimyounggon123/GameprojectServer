#ifndef _CENTERSERVER_H
#define _CENTERSERVER_H

#include "IOCPclass.h"	// already has pcc, emsg, etc.

#include "Packet.h"
#include "PacketCenterclass.h"		// center process

#include "ClientManager.h"

#define CENTER_PORT 9000
class CenterServer
	:public IOCPclass
{
	ClientManager* cm;
	PacketCenterclass& process;

	void packet_process(SOCKETINFO* ptr);
	void renew_data_length(SOCKETINFO* ptr, DWORD cbTransferred) override;
	void send_data(SOCKETINFO* ptr) override;
public:
	CenterServer()
		:IOCPclass(CENTER_PORT), process(PacketCenterclass::getInstance()), cm(new ClientManager)
	{}
	~CenterServer(){}

	VOID initialize() override;
};
#endif
