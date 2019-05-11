#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "server_index.h"
#include "server_lock.h"
#include "common_key.h"

IndexHandler::IndexHandler(std::string &file_name) {
	this->file_name = file_name;
	this->read_file.open(file_name);
	uint16_t n1, n2;
	std::string index, line, subject;
	std::getline(this->read_file, index);
	if (index.empty()) {
		this->current_index = 1;
	} else {
		this->current_index = std::stoi(index);
		while(std::getline(this->read_file, line)) {
			std::istringstream lineStream(line);
			std::getline(lineStream, subject, ';');
			lineStream >> n1 >> n2;
			Key k((uint8_t) n1, n2);
			this->private_map[subject] = k;
		}
	}
	this->read_file.close();
}

bool IndexHandler::has(std::string key) {
	Lock l(this->m);
	if (this->private_map.find(key) == this->private_map.end()) {
        return false;
	} else {
    	return true; 
	}
}

void IndexHandler::add(std::string user, Key &key) {
	Lock l(this->m);
	this->private_map[user] = key;
	this->current_index++;
}

int IndexHandler::get_next_index() {
	Lock l(this->m);
	int idx = this->current_index;
	return idx;
}

void IndexHandler::save() {
	std::map<std::string, Key>::iterator itr; 
	this->write_file.open(this->file_name);
	this->write_file << std::to_string(this->current_index) << std::endl;
	for (itr = this->private_map.begin(); 
			itr != this->private_map.end(); 
			itr++) { 
		this->write_file << 
			itr->first << "; " << 
			std::to_string(itr->second.get_exponent()) << 
			" " << 
			std::to_string(itr->second.get_modulus()) << 
			std::endl;
	}
}

Key IndexHandler::get_key(std::string user) {
	Lock l(this->m);
	Key k = this->private_map.at(user);
	return std::move(k);
}

void IndexHandler::remove(std::string user) {
	Lock l(this->m);
	this->private_map.erase(user);
}
