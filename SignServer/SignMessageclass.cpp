#include "SignMessageclass.h"
SignMessageclass* SignMessageclass::instance = nullptr;

void SignMessageclass::try_signup(const SOCKET_AND_ADDR& info, const char* ID, const char* PW) {
	print_time();
	_tprintf(_T("Sign-Up [Try](Socket: %d)(Port: %d) ID: %hs, password: %hs\n"),
		(int)info.sock, (int)ntohs(info.addr.sin_port), ID, PW);
}
void SignMessageclass::signup_done(const SOCKET_AND_ADDR& info, const char* ID, const char* PW) {
	print_time();
	_tprintf(_T("Sign-Up [Done](Socket: %d)(Port: %d) ID: %hs, password: %hs\n"),
		(int)info.sock, (int)ntohs(info.addr.sin_port), ID, PW);
}
void SignMessageclass::signup_fail(const SOCKET_AND_ADDR& info, const char* ID, const char* PW) {
	print_time();
	_tprintf(_T("Sign-Up [Fail](Socket: %d)(Port: %d) ID: %hs, password: %hs\n"),
		(int)info.sock, (int)ntohs(info.addr.sin_port), ID, PW);
}

void SignMessageclass::try_signin(const SOCKET_AND_ADDR& info, const char* ID, const char* PW) {
	print_time();
	_tprintf(_T("Sign-In [Try](Socket: %d)(Port: %d) ID: %hs, password: %hs\n"),
		(int)info.sock, (int)ntohs(info.addr.sin_port), ID, PW);
}
void SignMessageclass::signin_done(const SOCKET_AND_ADDR& info, const char* ID, const char* PW) {
	print_time();
	_tprintf(_T("Sign-In [Done](Socket: %d)(Port: %d) ID: %hs, password: %hs\n"),
		(int)info.sock, (int)ntohs(info.addr.sin_port), ID, PW);
}
void SignMessageclass::signin_fail(const SOCKET_AND_ADDR& info, const char* ID, const char* PW) {
	print_time();
	_tprintf(_T("Sign-In [Fail](Socket: %d)(Port: %d) ID: %hs, password: %hs\n"),
		(int)info.sock, (int)ntohs(info.addr.sin_port), ID, PW);
}
