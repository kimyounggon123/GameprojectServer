#ifndef _SINGLETONS_H
#define _SINGLETONS_H

////////////////////////////////////////////////////////////////////////////////////////////
// this struct saves class's singleton object except ServerAgentclass
////////////////////////////////////////////////////////////////////////////////////////////

/// <Neutral project>
#include "PacketControllerclass.h"
#include "ErrorMsgclass.h"
/// </Neutral project>


typedef struct _Singletons{
	/// <Neutral process>
	PacketControllerclass& pk_control;
	ErrorMsgclass& err_msg;
	/// </Neutral process>

	_Singletons() :
		pk_control(PacketControllerclass::getInstance()),
		err_msg(ErrorMsgclass::getInstance())
	{}

}Singletons;
#endif