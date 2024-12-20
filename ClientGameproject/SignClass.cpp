#include "SignClass.h"
SignClass* SignClass::instance = nullptr;

ERROR_CODE SignClass::select_sign_type(PK_TYPE* type, char* ID, char* PW) {
	if (type == nullptr || ID == nullptr || PW == nullptr) return ERROR_CODE::GET_NULLPTR;
	ERROR_CODE err_code; 
	char user_input[2]; // get user input
	_tprintf(_T("Sign up: 1, Log in: 2\n"));
	std::cin >> user_input;
	//if (fgets(user_input, 1, stdin) == NULL) return  ERROR_CODE::FUNCTION_RUNNING_FAILED;
	if (user_input[1] == '\n')
		user_input[1] = '\0'; // close string
	if (strcmp(user_input, "1") == 0) {
		err_code = do_sign_up(ID, PW);
		*type = PK_TYPE::TRY_SIGN_UP;
	}
	else if (strcmp(user_input, "2") == 0) {
		err_code = do_sign_in(ID, PW);
		*type = PK_TYPE::TRY_SIGN_IN;
	}
	else {
		printf("different: %s\n", user_input);
		err_code = ERROR_CODE::UNDEFINED_FUNCTION;
	}
	return err_code;
}

ERROR_CODE SignClass::do_sign_up(char* ID, char* PW){
	if (ID == nullptr || PW == nullptr) return ERROR_CODE::GET_NULLPTR;

	_tprintf(_T("[Sign up]\n"));
	_tprintf(_T("Type your ID: "));
	std::cin >> ID;
	size_t len = strlen(ID);
	if (len > 0 && ID[len - 1] == '\n') 
		ID[len - 1] = '\0';  // delete enter
	
	_tprintf(_T("Type your password: "));
	std::cin >> PW;
	len = strlen(PW);
	if (len > 0 && PW[len - 1] == '\n')
		PW[len - 1] = '\0';  // delete enter

	return ERROR_CODE::COMPLETE_THIS;
}
ERROR_CODE SignClass::do_sign_in(char* ID, char* PW) {
	if (ID == nullptr || PW == nullptr) return ERROR_CODE::GET_NULLPTR;

	
	_tprintf(_T("[Sign in]\n"));
	_tprintf(_T("Type your ID: "));

	std::cin >> ID;
	size_t len = strlen(ID);
	if (len > 0 && ID[len - 1] == '\n')
		ID[len - 1] = '\0';  // delete enter
	if (strlen(ID) == 0) return ERROR_CODE::EXCEED_SIZE;

	_tprintf(_T("Type your password: "));
	std::cin >> PW;
	len = strlen(PW);
	if (len > 0 && PW[len - 1] == '\n')
		PW[len - 1] = '\0';  // delete enter
	if (strlen(PW) == 0) return ERROR_CODE::EXCEED_SIZE;


	std::cout << "ID: " << ID << "PW: " << PW << std::endl;
	return ERROR_CODE::COMPLETE_THIS;
}
