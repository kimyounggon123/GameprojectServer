#ifndef _GAMEROOMSERVER_H
#define _GAMEROOMSERVER_H

#include "IOCPclass.h"
#include "PacketGameroomclass.h"

#include "ClientManager.h"
#include "GameroomBroadcast.h"
#define GAMEROOM_PORT 8002
class GameroomServer
	:public IOCPclass
{
	PacketGameroomclass& process;
	GameroomBroadcast& broadcaster;

	void packet_process(SOCKETINFO* ptr);

	void renew_data_length(SOCKETINFO* ptr, DWORD cbTransferred) override;
	void send_data(SOCKETINFO* ptr) override;
public:
	GameroomServer(USHORT room_num)
		:IOCPclass(GAMEROOM_PORT), process(PacketGameroomclass::getInstance()), broadcaster(GameroomBroadcast::getInstance())
	{}
	~GameroomServer(){}
	
	void initialize() override;

	void broadcast_for_all() override;
};
#endif

