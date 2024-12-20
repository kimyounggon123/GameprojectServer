#ifndef _PACKETPROCESSLIST_H
#define _PACKETPROCESSLIST_H

#include "PacketSignProcess.h"

class PacketProcessList
{

	PacketSignProcess& sign_proc;
	static PacketProcessList* instance;

	PacketProcessList(): sign_proc(PacketSignProcess::getInstance())
	{ }

public:
	static PacketProcessList& getInstance() {
		if (instance == nullptr) instance = new PacketProcessList;
		return *instance;
	}
	~PacketProcessList()
	{ }

	void initialize();
	std::function<ERROR_CODE(Packet*, USHORT*)> get_func(PK_TYPE type);
};

#endif

