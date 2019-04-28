#ifndef _SERVER_USERS
#define _SERVER_USERS

#include <fstream>
#include <unordered_map>
#include <iostream>

//using namespace std;

class ActiveUsers {

	int current_index;
	std::ifstream file;
	std::unordered_map<std::string, int> private_map;

	public:
	ActiveUsers(const char *file_name);

	bool has(std::string key);

	int get_next_index();

};


#endif