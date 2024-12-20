#ifndef _SIGNDBCONNECTOR_H
#define _SIGNDBCONNECTOR_H


#include "DBconnector.h"
#include "DBPKqueue.h"
class SignDBconnector
	:public DBconnector
{
	DBPKqueue& packet_queue;

	INT send_toDB() override;
	INT recv_fromDB() override;
public:
	SignDBconnector() :DBconnector(), packet_queue(DBPKqueue::getInstance())
	{}

	void initialize() override;
};
#endif
