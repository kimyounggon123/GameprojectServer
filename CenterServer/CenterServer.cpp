#include "CenterServer.h"

VOID CenterServer::initialize() {
	IOCPclass::initialize();
	process.initialize();
}

void CenterServer::packet_process(SOCKETINFO* ptr) {
	if (ptr == nullptr) return;
	PacketControllerclass& pcc = server_info->singletons->pk_control;
	ErrorMsgclass& emsg = server_info->singletons->err_msg;

	Packet pk;
	ERROR_CODE err = pcc.deserialize(ptr->buf, pk);


	if (err != ERROR_CODE::COMPLETE_THIS) {
		emsg.error_print(err, "deserialize()");
		return;
	}

	PK_TYPE type = static_cast<PK_TYPE>(pk.header.type);	
	auto func = process.get_func(type);
	if (!func) {
		emsg.err_display(_T("Non defined packet header!\n"));
		return;
	}
	
	err = func(&pk, ptr);	// it also remake packet. do serialize
	if (err != ERROR_CODE::COMPLETE_THIS) {
		emsg.error_print(err, "function()");
		return;
	}
	err = pcc.serialize(ptr->buf, pk);
	if (err != ERROR_CODE::COMPLETE_THIS) {
		emsg.error_print(err, "serialize()");
		return;
	}
}
void CenterServer::renew_data_length(SOCKETINFO* ptr, DWORD cbTransferred) {
	if(ptr == nullptr) return;
	// renew data length
	ptr->recvbytes = cbTransferred;
	ptr->sendbytes = 0;
	ptr->buf[ptr->recvbytes] = '\0'; // close buffer
	packet_process(ptr);
}
void CenterServer::send_data(SOCKETINFO* ptr) {
	if (ptr == nullptr) return;
	send_data_echo(ptr);
}
