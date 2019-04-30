#ifndef _SERVER_USERS
#define _SERVER_USERS

#include <fstream>
#include <unordered_map>
#include <iostream>

#include "common_key.h"

class ActiveUsers {

	int current_index;
	std::fstream file;
	std::ofstream f2;
	std::unordered_map<std::string, Key> private_map;

	public:
	ActiveUsers(const char *file_name);

	bool has(std::string key);

	void add(std::string user, Key &key);

	int get_next_index();

	void save();
};


#endif