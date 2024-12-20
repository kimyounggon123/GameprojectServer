#ifndef _USER_H
#define _USER_H

#include "stdafx.h"
#include "PositionStruct.h"
enum class USER_TYPE {
	YOU,
	NORMAL,
	FRIEND,
	BAD
};
#define USER_NAME_LIMITS 16
#define USER_PASSWORD_LIMITS 64
class User
{
	char name[USER_NAME_LIMITS + 1]; // + '\0'
	Position pos;
	USER_TYPE type;

public:
	User(const char* name, const USER_TYPE& type = USER_TYPE::NORMAL, const Position& pos = {0,0}) :
		type(type), pos(pos)
	{
		size_t get_name_length = strlen(name);
		strncpy_s(this->name, name, USER_NAME_LIMITS);
		if (get_name_length > USER_NAME_LIMITS)
			this->name[USER_NAME_LIMITS] = '\0';
		else
			this->name[get_name_length] = '\0';
	}
	User(std::string name, const USER_TYPE& type = USER_TYPE::NORMAL, const Position& pos = { 0,0 }) : 
		type(type), pos(pos)
	{
		size_t get_name_length = name.size();
		strncpy_s(this->name, name.c_str(), USER_NAME_LIMITS);
		if (get_name_length > USER_NAME_LIMITS)
			this->name[USER_NAME_LIMITS] = '\0';
		else
			this->name[get_name_length] = '\0';
	}
	const char* get_name() const { return name; }

	const USER_TYPE& get_user_type() const { return type; }
	void set_user_type(USER_TYPE type) { this->type = type; }

	const Position& get_pos() const { return pos; }
	void set_pos(const Position& moved) { pos = moved; }

	bool operator==(const User& other) const {
		return this->name == other.name;
	}

};

#endif