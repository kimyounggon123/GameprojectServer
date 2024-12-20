#include "PacketProcessList.h"

PacketProcessList* PacketProcessList::instance = nullptr;
void PacketProcessList::initialize() {
	sign_proc.initialize();
}


std::function<ERROR_CODE(Packet*, USHORT*)> PacketProcessList::get_func(PK_TYPE type) {
	std::function<ERROR_CODE(Packet*, USHORT*)> func;

	switch (type) {
	case PK_TYPE::SIGN_IN_COMPLETE:
		func = sign_proc.get_process(type);
		break;
	case PK_TYPE::SIGN_UP_COMPLETE:
		func = sign_proc.get_process(type);
		break;
	case PK_TYPE::SIGN_IN_FAIL:
		func = sign_proc.get_process(type);
		break;
	case PK_TYPE::SIGN_UP_FAIL:
		func = sign_proc.get_process(type);
		break;
	}


	return func;

}