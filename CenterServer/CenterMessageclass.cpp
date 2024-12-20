#include "CenterMessageclass.h"
CenterMessageclass* CenterMessageclass::instance = nullptr;

void CenterMessageclass::welcome_client(const char* ID, const SOCKETINFO& info){
	print_time();
	_tprintf(_T("Center [Come](Socket: %d)(Port: %d): %hs come to Center server\n"),
		(int)info.sa.sock, (int)ntohs(info.sa.addr.sin_port), ID);
}
void CenterMessageclass::bye_client(const char* ID, const SOCKETINFO& info){
	print_time();
	_tprintf(_T("Center [Leave](Socket: %d)(Port: %d): %hs leaved from Center server\n"),
		(int)info.sa.sock, (int)ntohs(info.sa.addr.sin_port), ID);
}
void CenterMessageclass::client_msg(const char* ID, const char* chat, const SOCKETINFO& info) {
	print_time();
	_tprintf(_T("Center [Chat](Socket: %d)(Port: %d): %hs: %hs\n"),
		(int)info.sa.sock, (int)ntohs(info.sa.addr.sin_port), ID, chat);
}
void CenterMessageclass::some_goto_gameroom(const char* ID, const SOCKETINFO& info) {
	print_time();
	_tprintf(_T("Center [Room](Socket: %d)(Port: %d): %hs move to Gameroom\n"),
		(int)info.sa.sock, (int)ntohs(info.sa.addr.sin_port), ID);
}