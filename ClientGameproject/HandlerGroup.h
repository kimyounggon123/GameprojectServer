#ifndef _HANDLERGROUP_H
#define _HANDLERGROUP_H

#include "PacketControllerclass.h"
#include "ErrorMsgclass.h"
#include "UserList.h"

#include "PacketProcessList.h"
typedef struct _HandlerGroup {
	PacketControllerclass& pcc;
	ErrorMsgclass& emsg;
	UserList& ul;
	PacketProcessList& proc;

	_HandlerGroup():
		pcc(PacketControllerclass::getInstance()),
		emsg(ErrorMsgclass::getInstance()),
		ul(UserList::getInstance()),
		proc(PacketProcessList::getInstance())
	{}

}HandlerGroup;

#endif