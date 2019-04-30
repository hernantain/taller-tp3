#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "server_users.h"
#include "common_key.h"

ActiveUsers::ActiveUsers(const char *file_name) {
	this->file.open(file_name, std::fstream::in | std::fstream::out);
	uint16_t n1, n2;
	std::string index, line, subject;
	std::getline(this->file, index);
	this->current_index = std::stoi(index);
	while(std::getline(this->file, line)) {
		std::istringstream lineStream(line);
		std::getline(lineStream, subject, ';');
		lineStream >> n1 >> n2;
		std::cout << n1 << n2 << std::endl;
		Key k((uint8_t) n1, n2);
		this->private_map[subject] = k;
	}

}

bool ActiveUsers::has(std::string key) {
	if (this->private_map.find(key) == this->private_map.end()) 
        return false;
    else
    	return true; 
}

void ActiveUsers::add(std::string user, Key &key) {
	this->private_map[user] = key;
	this->current_index += 1;
}

int ActiveUsers::get_next_index() {
	return this->current_index;
}

void ActiveUsers::save() {
	std::unordered_map<std::string, Key>:: iterator itr; 
	//this->file.seekp(1);
	this->f2.open("new_index.txt");
	this->f2 << std::to_string(this->current_index) << std::endl;
	std::cout << "INDEX: " << this->current_index << std::endl;
	for (itr = this->private_map.begin(); itr != this->private_map.end(); itr++) { 
		std::cout << itr->first << "; " << (int) itr->second.get_exponent() << " " << itr->second.get_modulus() << std::endl;
		this->f2 << itr->first << "; " << std::to_string(itr->second.get_exponent()) << " " << std::to_string(itr->second.get_modulus()) << std::endl;
	}
}

