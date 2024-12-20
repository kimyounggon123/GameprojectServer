#include "PKprocsBunch.h"
PKprocsBunch* PKprocsBunch::instance = nullptr;

ERROR_CODE PKprocsBunch::initialize() {
	sign_proc = new SignDBproc("localhost", "usermanager", "cyh153600!@", "users");
	if (!sign_proc) return ERROR_CODE::MEMORY_LIMIT;
	sign_proc->initialize();
	return ERROR_CODE::COMPLETE_THIS;
}


ERROR_CODE PKprocsBunch::do_proc(PK_TYPE type, Packet* pk_with_sender) {
	ERROR_CODE err_code = ERROR_CODE::OTHER_ERROR;
	std::function<ERROR_CODE(Packet*)> func = nullptr;

	switch (type) {
	case PK_TYPE::TRY_SIGN_IN:
		func = sign_proc->get_func(type);
		break;
	case PK_TYPE::TRY_SIGN_UP:
		func = sign_proc->get_func(type);
		break;
	}

	err_code = func(pk_with_sender);
	return err_code;
}