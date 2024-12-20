#ifndef _CLIENT_H
#define _CLIENT_H

#define CLIENT_NAME_LIMITS 16
#include "stdafx.h"
#include <WinSock2.h>
#include "SOCKETINFO.h"
#include "PositionStruct.h"
class Client
{
	char name[CLIENT_NAME_LIMITS + 1];
	int where_it_is; // gameroom number (0: default number)
	Position pos;

	//SOCKET sock;
	//SOCKADDR_IN addr;
	
	SOCKETINFO* sockinfo;
public:
	Client(const char* name, SOCKETINFO* sockinfo, const Position& pos = { 0,0 }) :
		sockinfo(sockinfo), pos(pos), where_it_is(0)
	{
		strncpy_s(this->name, name, CLIENT_NAME_LIMITS);
		this->name[CLIENT_NAME_LIMITS] = '\0';
	}
	/*Client(const char* name, SOCKETINFO* sockinfo, const Position& pos = {0,0}) :
		sock(sockinfo->sa.sock), addr(sockinfo->sa.addr), pos(pos)
	{
		strncpy_s(this->name, name, CLIENT_NAME_LIMITS);
		this->name[CLIENT_NAME_LIMITS] = '\0';
	}
	Client(std::string name, SOCKETINFO* sockinfo, const Position& pos = { 0,0 }) :
		sock(sockinfo->sa.sock), addr(sockinfo->sa.addr), pos(pos)
	{
		strncpy_s(this->name, name.c_str(), CLIENT_NAME_LIMITS);
		this->name[CLIENT_NAME_LIMITS] = '\0';
	}*/

	~Client() {
		delete sockinfo;
		sockinfo = nullptr;
	}
	const char* getName() const { return name; }

	SOCKETINFO& get_sockinfo() const { return *sockinfo; }
	const SOCKET& get_sock() const { return sockinfo->sa.sock; }
	const SOCKADDR_IN& get_addr() const { return sockinfo->sa.addr; }

	const Position& get_pos() const { return pos; }
	void set_pos(const Position& moved) { pos = moved; }

	int get_where() const { return where_it_is; }
	void move_other_room(int room_num) { where_it_is = room_num; }

	bool operator==(const Client& other) const {
		const SOCKADDR_IN& sock_addr = this->sockinfo->sa.addr;
		const SOCKADDR_IN& other_addr = other.sockinfo->sa.addr;
		if (sock_addr.sin_family != other_addr.sin_family) return false;
		if (ntohs(sock_addr.sin_port) != ntohs(other_addr.sin_port)) return false;
		if (sock_addr.sin_addr.s_addr != other_addr.sin_addr.s_addr) return false;
		if (strcmp(this->name, other.name) != 0) return false;

		return true;
	}
};

#endif