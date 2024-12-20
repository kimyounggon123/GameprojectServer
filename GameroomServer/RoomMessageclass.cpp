#include "RoomMessageclass.h"
RoomMessageclass* RoomMessageclass::instance = nullptr;

void RoomMessageclass::welcome_client(const char* ID, const SOCKETINFO& info){
	print_time();
	_tprintf(_T("Gameroom [Come](Socket: %d)(Port: %d): %hs come to Gameroom\n"),
		(int)info.sa.sock, (int)ntohs(info.sa.addr.sin_port), ID);
}
void RoomMessageclass::bye_client(const char* ID, const SOCKETINFO& info) {
	print_time();
	_tprintf(_T("Gameroom [Leave](Socket: %d)(Port: %d): %hs leaved from here\n"),
		(int)info.sa.sock, (int)ntohs(info.sa.addr.sin_port), ID);
}
void RoomMessageclass::leave_gameroom(const char* ID, const SOCKETINFO& info){
	print_time();
	_tprintf(_T("Gameroom [Center](Socket: %d)(Port: %d): %hs leaved from Gameroom\n"),
		(int)info.sa.sock, (int)ntohs(info.sa.addr.sin_port), ID);
}

void RoomMessageclass::someone_chat(const char* ID, const char* data, const SOCKETINFO& info){
	print_time();
	_tprintf(_T("Gameroom [Chat](Socket: %d)(Port: %d): %hs: %hs\n"),
		(int)info.sa.sock, (int)ntohs(info.sa.addr.sin_port), ID, data);
}
void RoomMessageclass::someone_move(const char* ID, const char* data, const SOCKETINFO& info){
	print_time();
	_tprintf(_T("Gameroom [Move](Socket: %d)(Port: %d): %hs move to here %hs\n"),
		(int)info.sa.sock, (int)ntohs(info.sa.addr.sin_port), ID, data);
}