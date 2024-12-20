#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "SignDBconnector.h"


void SignDBconnector::initialize() {
    ErrorMsgclass& emsg = singleton->err_msg;
    INT retval;
    DBconnector::initialize();

    // connect()
    ZeroMemory(&connector.addr, sizeof(connector.addr));
    connector.addr.sin_family = AF_INET;
    connector.addr.sin_addr.s_addr = inet_addr(BASIC_DB_SERVER_IP);
    connector.addr.sin_port = htons(BASIC_DB_PORT);
    retval = connect(connector.sock, (SOCKADDR*)&connector.addr, sizeof(connector.addr));
    if (retval == SOCKET_ERROR) emsg.err_quit(_T("connect IOCP()"));

    bind(connector.sock, (SOCKADDR*)&connector.addr, sizeof(connector.addr));
}

INT SignDBconnector::send_toDB() {
    ErrorMsgclass& emsg = singleton->err_msg;
    PacketControllerclass& pcc = singleton->pk_control;
    INT retval = 0;

    Packet* pk_send_toDB = packet_queue.bring_pkto_db(); // get packet from queue
    /*
    printf("pk_send_toDB Data (Length: %u): ", pk_send_toDB->header.length);
    for (size_t i = 0; i < pk_send_toDB->header.length; ++i) {
        printf("%02X ", pk_send_toDB->data[i]);
    }
    */
    if (pk_send_toDB == nullptr) {
        emsg.err_display(_T("Queue is empty: No packet to send."));
        return retval;
    }
  
    ERROR_CODE err_code = pcc.serialize(send_buf, *pk_send_toDB);
    if (err_code != ERROR_CODE::COMPLETE_THIS) {
        emsg.error_print(err_code, "SignDBconnector 37");
        return retval;
    }

    int len = pk_send_toDB->header.length + sizeof(pk_send_toDB->header);

    retval = send(connector.sock, send_buf, len, 0);
    printf("[connector] Send complete\n");

    if (pk_send_toDB)
        delete pk_send_toDB; // delete used packet
    return retval;
}



INT SignDBconnector::recv_fromDB() {
    ErrorMsgclass& emsg = singleton->err_msg;
    PacketControllerclass& pcc = singleton->pk_control;
    INT retval = 0;

    retval = recv(connector.sock, recv_buf, BUFFERSIZE, 0);
    if (retval == SOCKET_ERROR)  return retval;

    Packet* pk_get_fromDB = new Packet; // make a new packet
    if (pk_get_fromDB == nullptr) {
        emsg.error_print(ERROR_CODE::MEMORY_LIMIT);
        return -1;
    }

    ERROR_CODE result = pcc.deserialize(recv_buf, *pk_get_fromDB);
    if (result == ERROR_CODE::EXCEED_SIZE) {
        if (pk_get_fromDB) {
            delete pk_get_fromDB;
            pk_get_fromDB = nullptr;
        }
        emsg.error_print(result);
        return -1;
    }

    packet_queue.add_pkto_clients(pk_get_fromDB); // throw pk pointer to queue
    printf("[connector] Add a packet\n");
    
    return retval;
}