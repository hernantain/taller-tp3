#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "server_users.h"

ActiveUsers::ActiveUsers(const char *file_name) {
	this->file.open(file_name);
	std::string index, line, subject;
	std::getline(this->file, index);
	this->current_index = std::stoi(index);
	while(std::getline(this->file, line)) {
		std::istringstream lineStream(line);
		std::getline(lineStream, subject, ';');
		this->private_map[subject] = 0;
	}

}

bool ActiveUsers::has(std::string key) {
	if (this->private_map.find(key) == this->private_map.end()) 
        return false;
    else
    	return true; 
}

int ActiveUsers::get_next_index() {
	return this->current_index;
}
