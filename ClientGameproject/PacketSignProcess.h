#ifndef _PACKETSIGNPROCESS_H
#define _PACKETSIGNPROCESS_H


#include "PacketProcess.h"
class PacketSignProcess
	: public PacketProcess
{
	static PacketSignProcess* instance;
	PacketSignProcess(): PacketProcess(){}

	std::function<ERROR_CODE(Packet*, USHORT*)> sign_in_success;
	std::function<ERROR_CODE(Packet*, USHORT*)> sign_up_success;

	std::function<ERROR_CODE(Packet*, USHORT*)> sign_in_fail;
	std::function<ERROR_CODE(Packet*, USHORT*)> sign_up_fail;
public:
	static PacketSignProcess& getInstance() {
		if (instance == nullptr) instance = new PacketSignProcess;
		return *instance;
	}


	void initialize() override;
};
#endif
