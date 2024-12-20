#include "PacketMessageclass.h"

void PacketMessageclass::print_time() {
	time_t timer = time(NULL);
	struct tm t;
	localtime_s(&t, &timer);
	_tprintf(_T("(%d / %d / %d %d : %d : %d) "),
		t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
}
