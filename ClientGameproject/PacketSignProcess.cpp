#include "PacketSignProcess.h"

PacketSignProcess* PacketSignProcess::instance = nullptr;
void PacketSignProcess::initialize() {
	sign_in_success = [](Packet* pk, USHORT* result) {
		return ERROR_CODE::COMPLETE_THIS;
		};
	sign_up_success = [](Packet* pk, USHORT* result) {
		return ERROR_CODE::COMPLETE_THIS;
		};

	sign_in_fail = [](Packet* pk, USHORT* result) {
		return ERROR_CODE::COMPLETE_THIS;
		};
	sign_up_fail = [](Packet* pk, USHORT* result) {
		return ERROR_CODE::COMPLETE_THIS;
		};

	process[PK_TYPE::SIGN_IN_COMPLETE]  = sign_in_success;
	process[PK_TYPE::SIGN_IN_FAIL] = sign_in_fail;

	process[PK_TYPE::SIGN_UP_COMPLETE] = sign_up_success;
	process[PK_TYPE::SIGN_UP_FAIL] = sign_up_fail;
}