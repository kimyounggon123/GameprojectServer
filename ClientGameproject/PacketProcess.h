#ifndef _PACKETPROCESS_H
#define _PACKETPROCESS_H

#include "stdafx.h"
#include "ClientInfo.h" // key TOCONNECTSERVER, some macros
#include "Packet.h"
#include "UserList.h"
#include "PacketMessageClass.h"

#include "ErrorMsgclass.h"
#include <functional>
#include <map>

class PacketProcess
{
protected:
	UserList& ul;
	PacketMessageClass& pmsg;

	std::map<PK_TYPE, std::function<ERROR_CODE(Packet*, USHORT*)>> process;
public:
	PacketProcess() :ul(UserList::getInstance()), pmsg(PacketMessageClass::getInstance()) {}
	std::function<ERROR_CODE(Packet*, USHORT*)> get_process(PK_TYPE type);
	virtual void initialize() = 0;

};

#endif
