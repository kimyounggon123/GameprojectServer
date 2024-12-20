#ifndef _PKPROCSBUNCH_H
#define _PKPROCSBUNCH_H

#include "stdafx.h"
#include "SignDBproc.h"

enum class PROC_TYPE {
	DEFAULT, // default type, doesn't have any process
	SIGN
};
class PKprocsBunch
{
	SignDBproc* sign_proc;

	static PKprocsBunch* instance;
	PKprocsBunch(): sign_proc(nullptr)
	{}

public:
	static PKprocsBunch& getInstance() {
		if (instance == nullptr) instance = new PKprocsBunch;
		return *instance;
	}

	ERROR_CODE initialize();
	ERROR_CODE do_proc(PK_TYPE type, Packet* pk_with_sender);

	
};

#endif