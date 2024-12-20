#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "UserInfoManager.h"
QUERY_RESULT UserInfoManager::show_table() {
	if (!isInitialized) return QUERY_RESULT::INITIALIZE_ERROR;

	if (mysql_query(conn, "show tables")) return QUERY_RESULT::QUERY_ERROR;

	res = mysql_use_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s\n", row[0]);

	mysql_free_result(res);
	return QUERY_RESULT::COMPLETE;
}
QUERY_RESULT UserInfoManager::add_new_user(std::string ID, std::string password) {
	if (!isInitialized) return QUERY_RESULT::INITIALIZE_ERROR;

	// SQL 인젝션 방지를 위해 mysql_real_escape_string 사용
	char safeID[256];
	char safePassword[256];

	mysql_real_escape_string(conn, safeID, ID.c_str(), ID.length());
	mysql_real_escape_string(conn, safePassword, password.c_str(), password.length());


	// 수정된 SQL 쿼리
	std::string query =
		"INSERT INTO IDandPw (ID, PW) VALUES ('" + std::string(safeID) + "', '" + std::string(safePassword) + "')";

	// 쿼리 실행
	if (mysql_query(conn, query.c_str())) {
		return QUERY_RESULT::QUERY_ERROR;
	}

	// 쿼리 출력
	printf("==== Print ALL id and pw ====\n");
	query = "select* from idandpw;";
	if (mysql_query(conn, query.c_str())) {
		return QUERY_RESULT::QUERY_ERROR;
	}
	res = mysql_use_result(conn);
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s, %s\n", row[0], row[1]);

	mysql_free_result(res);
	return QUERY_RESULT::COMPLETE;
}
QUERY_RESULT UserInfoManager::check_user_exists(std::string ID, std::string password) {
	if (!isInitialized) return QUERY_RESULT::INITIALIZE_ERROR;
	// SQL 인젝션 방지를 위해 mysql_real_escape_string 사용
	char safeID[256];
	char safePassword[256];

	mysql_real_escape_string(conn, safeID, ID.c_str(), ID.length());
	mysql_real_escape_string(conn, safePassword, password.c_str(), password.length());

	std::string query =
		"SELECT ID FROM IDANDPW WHERE ID = '" + std::string(safeID) + "' AND PW = '" + std::string(safePassword) + "';";
	if (mysql_query(conn, query.c_str())) return QUERY_RESULT::QUERY_ERROR;

	res = mysql_use_result(conn);
	if ((row = mysql_fetch_row(res)) == NULL) return QUERY_RESULT::NO_RESULT;

	mysql_free_result(res);
	return QUERY_RESULT::COMPLETE;
}

QUERY_RESULT UserInfoManager::write_sign_in_log(std::string ID, const SOCKET_AND_ADDR& client_sockinfo) {
	if (!isInitialized) return QUERY_RESULT::INITIALIZE_ERROR;

	std::string port_str = std::to_string(client_sockinfo.sock);
	std::string IP_str = inet_ntoa(client_sockinfo.addr.sin_addr);

	std::string query =
		"INSERT INTO sign_in_log (userID, port, client_ip) VALUES ('" + ID + "', '" + port_str + "', '" + IP_str + "')";
	if (mysql_query(conn, query.c_str())) return QUERY_RESULT::QUERY_ERROR;

	// 쿼리 출력
	printf("==== Print All log in ====\n");
	query = "select* from sign_in_log;";
	if (mysql_query(conn, query.c_str())) {
		return QUERY_RESULT::QUERY_ERROR;
	}
	res = mysql_use_result(conn);
	// 쿼리 출력
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s  %s  %s  %s  %s\n", row[0], row[1], row[2], row[3], row[4]);
	mysql_free_result(res);
	return QUERY_RESULT::COMPLETE;
}