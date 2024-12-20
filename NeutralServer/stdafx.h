#ifndef _STDAFX_H
#define _STDAFX_H
//////////////////////////////////////////////////////////////////////////////////////////
// You can define macro and include frequently used header like std::vector in here.
//////////////////////////////////////////////////////////////////////////////////////////

#include <cstdint> // int_t
#define BUFFERSIZE 512

#define CENTER_PORT 9000
#define GAMEROOM_PORT 8002
#define SIGN_SERVERPORT 6000

#define BASIC_DB_SERVER_IP "127.0.1.1"
#define BASIC_DB_PORT 7000

#define CHAT_SERVER_PORT 5000

enum class ERROR_CODE : uint8_t {
	OTHER_ERROR, // default error code

	GET_NULLPTR,
	EXCEED_SIZE,
	HAVETO_DELETE_PACKET_HEADER,
	MEMORY_LIMIT,
	EMPTY_CONTAINOR,
	
	UNDEFINED_FUNCTION, // if you run undefined function
	FUNCTION_RUNNING_FAILED, // get function failed result 

	COMPLETE_THIS // if it doesn't have any error
};

#include <iostream>
#include <vector>
#include <algorithm>


#include <cstring>
#include <string>
#include <tchar.h> // _t macro
#endif