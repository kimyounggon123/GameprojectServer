#ifndef _USERLIST_H
#define _USERLIST_H

#include "User.h"


class UserList
{
	std::vector<User*>user_list;

	static UserList* instance;
	UserList() {}
public:
	static UserList& getInstance() {
		if (instance == nullptr) instance = new UserList;
		return *instance;
	}

	~UserList() {
		for (auto* user : user_list)
			delete user;
		user_list.clear();
	}

	User* find_user(const char* name) {
		User* found = nullptr;
		for (auto* user : user_list) {
			if (user->get_name() == name) {
				found = user;
				break;
			}
		}
		return found;
	}

	User* find_user(const User& other) {
		User* found = nullptr;
		for (auto* user : user_list) {
			if (user == &other) {
				found = user;
				break;
			}
		}
		return found;
	}

	User* find_user(const User* other) {
		User* found = nullptr;
		for (auto* user : user_list) {
			if (user == other) {
				found = user;
				break;
			}
		}
		return found;
	}


	bool add_user(User* user) {
		if (user == nullptr) return false;
		user_list.push_back(user);
		return true;
	}

	bool del_user(const User& user) {
		bool result = false;
		for (auto it = user_list.begin(); it != user_list.end(); it++) {
			if (*it == &user) {
				delete *it;
				user_list.erase(it);
				result = true; break;
			}
		}
		return result;
	}
};
#endif
