#ifndef _SERVER_ENCRYPTER
#define _SERVER_ENCRYPTER

#include <string>
#include <sstream>
#include <cstdint>

#include "common_key.h"

class Encrypter {

	uint8_t private_exponent, public_exponent;
	uint16_t private_modulus, public_modulus;
	uint32_t hash_calculated, hash_encrypted_private;
	std::string certificate;


	uint32_t rsa(uint32_t num, uint8_t exponent, uint16_t mod);

	uint32_t encrypt_with_private_key();
	
	uint32_t encrypt_with_public_key(uint32_t &tmp_huella);

	public:
		//CERT  PRI  PUBLIC
	Encrypter(std::string &cert, Key &private_key, Key &public_key);

	void calculate_hash();

	uint32_t get_calculated_hash();

	uint32_t get_hash_encrypted_with_private();

	uint32_t encrypt();

	uint32_t decrypt(uint32_t print);

};


#endif