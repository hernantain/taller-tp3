#include <iostream>
#include <cstdint>

#include "common_encrypter.h"

							//CERT 				PRI 				PUBLIC
Encrypter::Encrypter(std::string &cert, uint8_t &s_exp, uint8_t &cli_exp) {
	this->certificate = cert;
	this->server_exponent = s_exp;
	this->client_exponent = cli_exp;
	this->modulus = 253;
	this->hash_calculated = 0;
	std::cout << "Clave Privada: (" << (int) this->server_exponent << ", " << this->modulus << ")" << std::endl;
	std::cout << "Clave Publica: (" << (int) this->client_exponent << ", " << this->modulus << ")" << std::endl;
}

void Encrypter::calculate_hash() {
	std::istringstream stream(this->certificate);
	char c;
	while (stream >> std::noskipws >> c) {
		this->hash_calculated += (int) c;
	}
}

uint32_t Encrypter::get_calculated_hash() {
	return this->hash_calculated;
}


uint32_t Encrypter::rsa(uint32_t num, uint8_t exponent, uint16_t mod) {
	unsigned char byte = (char) num;
	uint32_t sum = 0;

	for (int i = 0; i < 2; ++i) {
		int res = 1;
		for (unsigned char j = 1; j <= exponent; ++j) {
			res = (res*byte) % mod;
		}
		byte = num >> 8;
		(i == 0) ? sum += res : sum += res*256;
	}

	return sum;
}


uint32_t Encrypter::encrypt_with_private_key() {
	return this->rsa(this->hash_calculated, this->server_exponent, this->modulus);
}

uint32_t Encrypter::encrypt_with_public_key(uint32_t &tmp_huella) {
	return this->rsa(tmp_huella, this->client_exponent, this->modulus);
}

uint32_t Encrypter::encrypt() {
	uint32_t huella = this->encrypt_with_private_key();
	return this->encrypt_with_public_key(huella);
}

uint32_t Encrypter::decrypt(uint32_t print) {
	uint32_t huella = this->rsa(print, this->server_exponent, this->modulus);
	return this->encrypt_with_public_key(huella);
}