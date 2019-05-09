#include <cstdint>
#include <fstream>
#include <string>

#include "client_info.h"

#define THIRTY_DAYS 2592000 // 24 * 60 * 60 * 30

ClientInfo::ClientInfo(std::string file_name) {
	this->file.open(file_name);
	std::getline(this->file, this->name);

	if (this->file.eof()) {
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		char buf[20];

		strftime(buf, sizeof(buf), "%b %d %H:%M:%S %Y", now);
		this->start_date = buf;

		t += THIRTY_DAYS;
		std::tm* aft = std::localtime(&t);

		strftime(buf, sizeof(buf), "%b %d %H:%M:%S %Y", aft);

		this->end_date = buf;
	} else {
		std::getline(this->file, this->start_date);
		std::getline(this->file, this->end_date);
	}
}

std::string ClientInfo::get_name() {
	return this->name;
}

std::string ClientInfo::get_start_date() {
	return this->start_date;
}

std::string ClientInfo::get_end_date() {
	return this->end_date;
}

ClientInfo::~ClientInfo() {
	this->file.close();
}
