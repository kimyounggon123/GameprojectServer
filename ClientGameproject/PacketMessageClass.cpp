#include "PacketMessageClass.h"

PacketMessageClass* PacketMessageClass::instance = nullptr;
void PacketMessageClass::print_time() {
	time_t timer = time(NULL);
	struct tm t;
	localtime_s(&t, &timer);
	_tprintf(_T("(%d / %d / %d %d : %d : %d) "),
		t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
}
void PacketMessageClass::welcome_client(const char* ID) {
	print_time();
	_tprintf(_T("Hello <%hs> !\n"), ID);
}
void PacketMessageClass::bye_client(const char* ID) {
	print_time();
	_tprintf(_T("Bye <%hs> !\n"), ID);
}
void PacketMessageClass::someone_chat(const char* ID, const char* chat) {
	print_time();
	_tprintf(_T("[%hs]: %hs\n"), ID, chat);
}
void PacketMessageClass::someone_move(const char* ID, const char* pos_buf) {
	print_time();
}