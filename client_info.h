#ifndef _CLIENT_INFO
#define _CLIENT_INFO

#include <cstdint>
#include <fstream>
#include <ctime>


class ClientInfo {
	std::string name, start_date, end_date;
	std::ifstream file;

	public:
	ClientInfo(std::string file_name);

	std::string get_name();

	std::string get_start_date();

	std::string get_end_date();

	~ClientInfo();
};

#endif
