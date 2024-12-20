#ifndef _PROCESSAGENT_H
#define _PROCESSAGENT_H

#include "ClientAgent.h"
class ProcessAgent
	:public ClientAgent
{

public:
	//ProcessAgent();
	//~ProcessAgent();



	void initialize() override;
	void run() override;
};

#endif