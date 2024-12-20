#ifndef _PROCESSCLASS_H
#define _PROCESSCLASS_H

#include "stdafx.h"
#include <conio.h>

#include "Packet.h"
#include "PacketControllerclass.h"
class ProcessClass
{
	static ProcessClass* instance;
	PacketControllerclass& pcc;
	ProcessClass():pcc(PacketControllerclass::getInstance()){}

public:
	static ProcessClass& getInstance() {
		if (instance == nullptr) instance = new ProcessClass;
		return *instance;
	}


	ERROR_CODE chat(Packet* pk);
	ERROR_CODE move(Packet* pk);

};
#endif
