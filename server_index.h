#ifndef _SERVER_USERS
#define _SERVER_USERS

#include <fstream>
#include <unordered_map>
#include <iostream>

#include "common_key.h"

class IndexHandler {

	int current_index;
	std::string file_name;
	std::ifstream read_file;
	std::ofstream write_file;
	std::unordered_map<std::string, Key> private_map;

	public:
	IndexHandler(std::string &file_name);

	bool has(std::string key);

	void add(std::string user, Key &key);

	int get_next_index();

	Key get_key(std::string user);

	void remove(std::string user);

	void save();
};

#endif
