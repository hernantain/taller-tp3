#ifndef _COMMON_CERTIFICATE
#define _COMMON_CERTIFICATE

#include <cstdint>
#include <string>

class Certificate {
	std::string get_serial_number_with_format();
	std::string get_subject_with_format();
	std::string get_issuer_with_format(); 
	std::string get_start_date_with_format();
	std::string get_end_date_with_format(); 
	std::string get_modulus_with_format();
	std::string get_exponent_with_format();
	std::string get_file_name();

	uint32_t get_number(std::string &num);

	public:
	std::string issuer = "Taller de programacion 1";
	uint8_t client_exponent;
	uint16_t client_modulus;
	uint32_t subject_len, serial_number;
	std::string subject, start_date, end_date;

	void save();

	std::string operator()();

	Certificate();

	explicit Certificate(std::string &file_name);
};


#endif

