#include "DBPKqueue.h"


DBPKqueue* DBPKqueue::instance = nullptr;
ERROR_CODE DBPKqueue::add_pkto_db(Packet* pk) {
	if (pk == nullptr) return ERROR_CODE::GET_NULLPTR;

	PK_TYPE pk_header = static_cast<PK_TYPE>(pk->header.type);	// Get pk header type
	if (pk_header == PK_TYPE::BE_DELETED) return ERROR_CODE::HAVETO_DELETE_PACKET_HEADER;

	to_db_list.push(pk);
	SetEvent(DBevent); // event signal mode
	return ERROR_CODE::COMPLETE_THIS;
}

Packet* DBPKqueue::bring_pkto_db() {
	WaitForSingleObject(DBevent, INFINITE);

	if (to_db_list.empty()) return nullptr;  // Safety check

	Packet* bring_this = to_db_list.front();
	to_db_list.pop(); // pop front, you don't delete that

	if (to_db_list.empty())
		ResetEvent(DBevent);
	
	return bring_this;
}
ERROR_CODE DBPKqueue::add_pkto_clients(Packet* pk_from_db) {
	if (pk_from_db == nullptr)	return ERROR_CODE::GET_NULLPTR; // if you cannot make packet.
	db_info_clients.push(pk_from_db);

	SetEvent(ClientEvent); // event signal mode
	return ERROR_CODE::COMPLETE_THIS;
}
Packet* DBPKqueue::bring_pkto_client() {
	WaitForSingleObject(ClientEvent, INFINITE);
	if (db_info_clients.empty()) return nullptr;

	Packet* bring_this = db_info_clients.front();
	db_info_clients.pop(); // pop front, you don't delete that

	if (db_info_clients.empty())
		ResetEvent(ClientEvent);
	return bring_this;
}