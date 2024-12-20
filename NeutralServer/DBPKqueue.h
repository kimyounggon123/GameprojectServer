#ifndef _DBPKQUEUE_H
#define _DBPKQUEUE_H

#include "stdafx.h"
#include <queue>

#include "Packet.h"
#include "PacketControllerclass.h"


class DBPKqueue
{
	std::queue<Packet*>to_db_list;		// connector -> db
	std::queue<Packet*>db_info_clients;	// server -> client

	HANDLE ClientEvent;
	HANDLE DBevent;

	static DBPKqueue* instance;
	DBPKqueue() {
		DBevent = CreateEvent(NULL, TRUE, FALSE, NULL);
		ClientEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

public:
	 ~DBPKqueue() {
		while (!to_db_list.empty()) {
			delete to_db_list.front();
			to_db_list.pop();
		}
		CloseHandle(DBevent);
		CloseHandle(ClientEvent);
	}

	 static DBPKqueue& getInstance() {
		 if (instance == nullptr) instance = new DBPKqueue;
		 return *instance;
	 }
	// it'll be sended to db server
	// you have to delete this at server methods.
	// use it at connector
	ERROR_CODE add_pkto_clients(Packet* pk_from_db);
	Packet* bring_pkto_db();

	// it'll be sended to the client
	// you have to delete this at server methods.
	// use it at server
	ERROR_CODE add_pkto_db(Packet* pk);
	Packet* bring_pkto_client();
};
#endif
