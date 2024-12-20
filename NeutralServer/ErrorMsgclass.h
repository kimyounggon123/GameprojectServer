#ifndef _ERRORMSGCLASS_H
#define _ERRORMSGCLASS_H

#include "stdafx.h"
#include <Windows.h>

#include <map>
#include <functional>

class ErrorMsgclass
{

	void get_nullptr(const char* where);
	void exceed_size(const char* where);
	void haveto_delete_pk_header(const char* where);
	void memory_limit(const char* where);
	void undefined_func_call(const char* where);

	static ErrorMsgclass* instance;
	ErrorMsgclass() {}
public:
	static ErrorMsgclass& getInstance() {
		if (instance == nullptr) instance = new ErrorMsgclass;
		return *instance;
	}

	VOID err_quit(LPCTSTR Msg) {
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (LPTSTR)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (LPCTSTR)lpMsgBuf, Msg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		exit(1);
	}
	VOID err_display(LPCTSTR Msg) {
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
		std::wcout << "\n[" << Msg << "]: " << GetLastError() << (TCHAR*)lpMsgBuf;
		LocalFree(lpMsgBuf);
	}

	void error_print(ERROR_CODE code, const char* where = "");
};
#endif