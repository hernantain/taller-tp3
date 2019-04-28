#ifndef _SERVER_ENCRYPTER
#define _SERVER_ENCRYPTER

#include <string>
#include <sstream>
#include <cstdint>

class Encrypter {

	uint8_t server_exponent, client_exponent;
	uint16_t modulus;
	uint32_t hash_calculated;
	std::string certificate;

	uint32_t rsa(uint32_t num, uint8_t exponent, uint16_t mod);

	uint32_t encrypt_with_private_key();
	
	uint32_t encrypt_with_public_key(uint32_t &tmp_huella);

	public:
	Encrypter(std::string &cert, uint8_t &s_exp, uint8_t &cli_exp);

	void calculate_hash();

	uint32_t get_calculated_hash();

	uint32_t encrypt();

	uint32_t decrypt(uint32_t print);

};


#endif