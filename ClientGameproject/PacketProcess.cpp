#include "PacketProcess.h"
std::function<ERROR_CODE(Packet*, USHORT*)>  PacketProcess::get_process(PK_TYPE type) {
	auto func = process[type];
	if (!func) {
		return [this](Packet* pk, USHORT* port_num) {
			return ERROR_CODE::UNDEFINED_FUNCTION;
		};
	}
	return process[type]; 
}
