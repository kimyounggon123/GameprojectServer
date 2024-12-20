#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ClientAgent.h"
#include <conio.h>
gKey key;

ClientAgent::ClientAgent():exit_flag(false), proc(ProcessClass::getInstance()), sign_info(SignClass::getInstance()){
    handler = new HandlerGroup();
    ErrorMsgclass& emsg = handler->emsg;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return;
}
ClientAgent::~ClientAgent() {
    closesocket(connector.sock);
    CloseHandle(key.hEvent);
    CloseHandle(key.hMutex);

    CloseHandle(send_event);
    CloseHandle(recv_event);
    WSACleanup();
    delete handler;
    handler = nullptr;
}

void ClientAgent::initialize() {
    reset_portinfo(SIGN_SERVERPORT); // to enter log in server


    send_event = CreateEvent(NULL, TRUE, FALSE, NULL);;
    recv_event = CreateEvent(NULL, TRUE, FALSE, NULL);;

    key.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    key.hMutex = CreateMutex(NULL, FALSE, NULL);

    thread_info.hThreads[0] = 
        (HANDLE)_beginthreadex(
            NULL, 0,
            process_thread, this,
            0, (unsigned*)&thread_info.dwThreadID[0]
            );
    if (thread_info.hThreads[0] == NULL) handler->emsg.err_quit(_T("_beginthreadex()"));

    thread_info.hThreads[1] =
        (HANDLE)_beginthreadex(
            NULL, 0,
            update_thread, this,
            0, (unsigned*)&thread_info.dwThreadID[1]
        );
    if (thread_info.hThreads[1] == NULL) handler->emsg.err_quit(_T("_beginthreadex()"));

    // initialize function in PacketProcess
    handler->proc.initialize();
}

bool ClientAgent::log_in_process() {
    bool log_complete = false;
    INT retval; // send and recv
    
    ErrorMsgclass& emsg = handler->emsg;
    UserList& ul = handler->ul;

    PacketControllerclass& pcc = handler->pcc;
    char pk_buf[BUFFERSIZE + 1];

    ERROR_CODE err_code;

    Packet* sign_packet = new Packet;
    while (!log_complete) {
        try {
            system("cls");
            char id_input[USER_NAME_LIMITS + 1]; // input player id
            char pw_input[USER_PASSWORD_LIMITS + 1]; // input player password
            memset(id_input, 0, USER_NAME_LIMITS + 1);
            memset(pw_input, 0, USER_PASSWORD_LIMITS + 1);

            // 1. make player's id and password
            PK_TYPE sign_type = PK_TYPE::BE_DELETED; // set sign type
            err_code = sign_info.select_sign_type(&sign_type, id_input, pw_input); // type player's id and password
            if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code; // if it failed

            // 3. Set sign packet
            err_code = pcc.set_packet(sign_packet, sign_type, id_input);
            if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code;
            err_code = pcc.add_data_in_packet(sign_packet, pw_input);
            if (err_code != ERROR_CODE::COMPLETE_THIS) throw err_code;

            // 4. serialize packet
            err_code = pcc.serialize(pk_buf, *sign_packet);
            if (err_code != ERROR_CODE::COMPLETE_THIS) {
                emsg.err_display(_T("serialize()"));
                throw err_code;
            }
            // 5. send packet data
            retval = send(connector.sock, pk_buf, static_cast<uint16_t>(sizeof(sign_packet->header)) + sign_packet->header.length, 0);
            if (retval == SOCKET_ERROR) {
                emsg.err_display(_T("send()"));
                throw ERROR_CODE::FUNCTION_RUNNING_FAILED;
            }

            // 6. receive packet data from server
            retval = recv(connector.sock, pk_buf, BUFFERSIZE, 0);
            if (retval == 0) {
                printf("recv 반환 값 0\n");
                throw ERROR_CODE::FUNCTION_RUNNING_FAILED;
            }
            if (retval == SOCKET_ERROR) {
                int code = WSAGetLastError();
                printf("recv() 실패, 에러 코드: %d\n", err_code);;
                emsg.err_display(_T("recv()"));
                throw ERROR_CODE::FUNCTION_RUNNING_FAILED;
            }
            
            // Get and use packet
            err_code = pcc.deserialize(pk_buf, *sign_packet);
            if (err_code != ERROR_CODE::COMPLETE_THIS) {
                emsg.err_display(_T("deserialize() Sign"));
                throw err_code;
            }

            // 7. Do appropriate process
            err_code = process_packet_from_server(pk_buf, *sign_packet);
            if (err_code != ERROR_CODE::COMPLETE_THIS) {
                emsg.err_display(_T("func()"));
                throw ERROR_CODE::FUNCTION_RUNNING_FAILED;
            }

            sign_type = static_cast<PK_TYPE>(sign_packet->header.type);
            if (sign_type == PK_TYPE::SIGN_IN_COMPLETE) {
                log_complete = true;
                USHORT port_num_got;
                memcpy(&port_num_got, sign_packet->data, sizeof(USHORT));


                sign_info.make_you(id_input); // make your info

               
                reset_portinfo(port_num_got);

            }

        }
        catch (ERROR_CODE code) {
            emsg.error_print(code);
            continue;
        }
    }

    SetEvent(recv_event);
    SetEvent(send_event);
    return log_complete;
}

VOID ClientAgent::run() {
    bool do_log_in = log_in_process();
    if (!do_log_in) return;
    system("cls");
    printf("로그인 성공! \n");
    //WaitForSingleObject(thread_info.hThreads[0], INFINITE);
    WaitForMultipleObjects(THREAD_NUM, thread_info.hThreads, TRUE, INFINITE);
}

bool ClientAgent::reset_portinfo(USHORT port_num) {
    ErrorMsgclass& emsg = handler->emsg;
    if (connector.sock)
    {
        closesocket(connector.sock);
    }
    INT retval;
    // move server by server
    connector.sock = socket(AF_INET, SOCK_STREAM, 0);
    if (connector.sock == SOCKET_ERROR)  emsg.err_quit(_T("socket() change error"));

    ZeroMemory(&connector.server_address, sizeof(connector.server_address));
    connector.server_address.sin_family = AF_INET;
    connector.server_address.sin_addr.s_addr = inet_addr(SERVERIP);
    //connector.server_address.sin_addr.s_addr = inet_addr("127.0.1.1");
    connector.server_address.sin_port = htons(port_num);
    retval = connect(connector.sock, (SOCKADDR*)&connector.server_address, sizeof(connector.server_address));
    if (retval == SOCKET_ERROR) emsg.err_quit(_T("connect() change error"));
    /*

    // 서버 접속용 커넥팅
    const char* yourName = sign_info.getYou()->get_name();
    Packet* pk = new Packet(PK_TYPE::ENTER_NEW_SERVER, yourName);
    int pk_len = pk->header.length + sizeof(PacketHeader);
    char* pk_buf = new char[pk_len + 1];

    try {
        ERROR_CODE err = handler->pcc.serialize(pk_buf, *pk);
        if (err != ERROR_CODE::COMPLETE_THIS)
            throw err;
        
        retval = send(connector.sock, pk_buf, pk_len, 0);
        if (retval == SOCKET_ERROR) {
            emsg.err_display(_T("serialize()"));
            throw ERROR_CODE::FUNCTION_RUNNING_FAILED;
        }

    }
    catch (ERROR_CODE err) {
        emsg.error_print(err);
    }
    if (pk) {
        delete pk; pk = nullptr;
    }
    if (pk_buf) {
        delete[] pk_buf; pk_buf = nullptr;
    }
    */
    return true;
}

ERROR_CODE ClientAgent::process_packet_from_server(const char* got_pk, Packet& packet) {
    ErrorMsgclass& emsg = handler->emsg;
    PacketControllerclass& pcc = handler->pcc;
    PacketProcessList& proc = handler->proc;
    bool is_remade;
    USHORT result = 0;
    ERROR_CODE err_code;
    try {
        err_code = pcc.deserialize(got_pk, packet);			            // deserialize
        if (err_code != ERROR_CODE::COMPLETE_THIS) {
            emsg.err_display(_T("deserialize()"));
            throw err_code;
        }
        PK_TYPE header = static_cast<PK_TYPE>(packet.header.type);			// get Header
        auto func = proc.get_func(header);	// get function that match with header
        if (!func) {
            emsg.err_display(_T("undefined function()"));
            throw ERROR_CODE::UNDEFINED_FUNCTION;
        }
        err_code = func(&packet, &result);
        if (err_code != ERROR_CODE::COMPLETE_THIS) {
            emsg.err_display(_T("func()"));
            throw err_code;
        }
    }
    catch (ERROR_CODE err) {
        emsg.error_print(err);
        is_remade = false;
    }
    switch (result) {
    case 0:
        break;
    default: // Got server port 
        reset_portinfo(result);
        break;
    }

    return err_code;
}

/// <IOCP thread>
unsigned int WINAPI ClientAgent::update_thread(LPVOID lpParam) {
    ClientAgent* This = static_cast<ClientAgent*>(lpParam);
    HandlerGroup* handler = This->handler;
    ErrorMsgclass& emsg = handler->emsg;
    PacketControllerclass& pcc = handler->pcc;
   
    TOCONNECTSERVER& connector = This->connector;

    /// <for recv>
    Packet pk;
    INT retval;
    char pk_buf[BUFFERSIZE + 1];
    ERROR_CODE err_code;
    /// </for recv>

    WaitForSingleObject(This->recv_event, INFINITE);
    while (!This->exit_flag) {
        try {
            memset(pk_buf, 0, BUFFERSIZE);  // 버퍼 초기화
            // std::cout << "hello " << std::endl;
            retval = recv(connector.sock, pk_buf, BUFFERSIZE, 0);
            if (retval == SOCKET_ERROR) { 
                emsg.err_display(_T("recv()"));
                throw ERROR_CODE::FUNCTION_RUNNING_FAILED;
            }
            PacketHeader header;
            memcpy(&header, pk_buf, sizeof(PacketHeader));
            printf("길이: %d\n", header.length);
            printf("헤더: %d\n", header.type);
            if (retval > 0) {
                printf("받은 데이터: ");
                for (int i = 0; i < retval; ++i) {
                    printf("%02X ", pk_buf[i]);
                }
                printf("\n");
            }

            else if (retval == 0) {
                printf("서버가 연결을 종료했습니다.\n");
            }
            err_code = This->process_packet_from_server(pk_buf, pk);
            if (err_code != ERROR_CODE::COMPLETE_THIS) {
                emsg.err_display(_T("process_packet_from_server()"));
                throw err_code;
            }
        }
        catch (ERROR_CODE err) {
            emsg.error_print(err);
        }
    }
    return 0;
}

unsigned int WINAPI ClientAgent::process_thread(LPVOID lpParam) {
    ClientAgent* This = static_cast<ClientAgent*>(lpParam);
    HandlerGroup* handler = This->handler;

    ErrorMsgclass& emsg = handler->emsg;
    PacketControllerclass& pcc = handler->pcc;

    TOCONNECTSERVER& connector = This->connector;

    // make variable data structure

    /// <for send>
    INT retval;
    int len;
    char pk_buf[BUFFERSIZE + 1];
    ERROR_CODE err_code;
    /// </for send>

    WaitForSingleObject(This->send_event, INFINITE);
    while (!This->exit_flag) {
        try {
            if (_kbhit()) {
                int ch = _getch();

                ///// make packet
                Packet* pk = new Packet;
                err_code = This->process_result(pk, ch); 
                if (err_code != ERROR_CODE::COMPLETE_THIS) {
                    emsg.err_display(_T("process_result()"));
                    throw err_code;
                }
                

                ///// make packet
        
                
                /// Add id part
                err_code = pcc.add_data_in_packet(pk, This->sign_info.getYou()->get_name()); // 여기 문제 있음.
                if (err_code != ERROR_CODE::COMPLETE_THIS) {
                    emsg.err_display(_T("add_data_in_packet()"));
                    throw err_code;
                }
                
                /// Add id part

                err_code = pcc.serialize(pk_buf, *pk);
                if (err_code != ERROR_CODE::COMPLETE_THIS) {
                    emsg.err_display(_T("serialize()"));
                    throw err_code;
                }

               
                len = pk->header.length + sizeof(PacketHeader);
                retval = send(connector.sock, pk_buf, len, 0);
                if (retval == SOCKET_ERROR) {
                    emsg.err_display(_T("serialize()"));
                    throw ERROR_CODE::FUNCTION_RUNNING_FAILED;
                }
                if (retval > 0) {
                    printf("전달한 데이터: ");
                    for (int i = 0; i < retval; ++i) {
                        printf("%02X ", pk_buf[i]);
                    }
                    printf("\n");
                }
                delete pk; pk = nullptr;
            }
        }
        catch (ERROR_CODE err) {
            emsg.error_print(err);
            continue;
        }
    }

    return 0;
}
ERROR_CODE ClientAgent::process_result(Packet* pk, int ch) {
    ERROR_CODE err_code = ERROR_CODE::OTHER_ERROR;
    if (ch == '\r') {
        err_code = proc.chat(pk);

        printf("%d", static_cast<uint16_t>(err_code));
    }
 
    // arrow keys
    if (ch == 0 || ch == 224) {
        int ch_new = _getch();
        switch (ch_new) {
        case 72:
            break;
        case 80:
            break;
        case 75:
            break;
        case 77:
            break;
        }
    }

    return err_code;
}
/// </IOCP thread>

