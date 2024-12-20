#include "ErrorMsgclass.h"
ErrorMsgclass* ErrorMsgclass::instance = nullptr;

void ErrorMsgclass::get_nullptr(const char* where) {
	_tprintf(_T("(%hs) [GET_NULLPTR] Parameter gets nullptr.\n"), where);
}
void ErrorMsgclass::exceed_size(const char* where) {
	_tprintf(_T("(%hs) [EXCEED_SIZE] Buffer gets too many informations.\n"), where);
}
void ErrorMsgclass::haveto_delete_pk_header(const char* where) {
	_tprintf(_T("(%hs) [HAVETO_DELETE_PACKET_HEADER] You have to delete this packet.\n"), where);
}
void ErrorMsgclass::memory_limit(const char* where) {
	_tprintf(_T("(%hs) [MEMORY_LIMIT] Parameter gets nullptr! \n"), where);
}
void ErrorMsgclass::undefined_func_call(const char* where) {
	_tprintf(_T("(%hs) [UNDEFINED_FUNCTION] You call a undefined function! \n"), where);
}
void ErrorMsgclass::error_print(ERROR_CODE code, const char* where) {
	switch (code) {
	case ERROR_CODE::GET_NULLPTR:
		get_nullptr(where);
		break;
	case ERROR_CODE::EXCEED_SIZE:
		exceed_size(where);
		break;
	case ERROR_CODE::HAVETO_DELETE_PACKET_HEADER:
		haveto_delete_pk_header(where);
		break;
	case ERROR_CODE::MEMORY_LIMIT:
		memory_limit(where);
		break;
	case ERROR_CODE::UNDEFINED_FUNCTION:
		undefined_func_call(where);
		break;
	default:
		_tprintf(_T("[UNKNOWN] Unknown error.\n"));
		break;
	}
}