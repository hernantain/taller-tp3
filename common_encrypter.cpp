#include <iostream>
#include <cstdint>

#include "common_encrypter.h"
#include "common_key.h"


Encrypter::Encrypter(std::string &cert, Key &private_key, Key &public_key) {
	this->certificate = cert;
	this->public_exponent = public_key.get_exponent();
	this->private_exponent = private_key.get_exponent();
	this->public_modulus = public_key.get_modulus();
	this->private_modulus = private_key.get_modulus();
	this->hash_calculated = 0;
	std::cout << "Clave Privada: (" << (int) this->private_exponent << ", " << this->private_modulus << ")" << std::endl;
	std::cout << "Clave Publica: (" << (int) this->public_exponent << ", " << this->public_modulus << ")" << std::endl;
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
	return this->rsa(this->hash_calculated, this->private_exponent, this->private_modulus);
}

uint32_t Encrypter::encrypt_with_public_key(uint32_t &tmp_huella) {
	return this->rsa(tmp_huella, this->public_exponent, this->public_modulus);
}

uint32_t Encrypter::encrypt() {
	uint32_t huella = this->encrypt_with_private_key();
	return this->encrypt_with_public_key(huella);
}

uint32_t Encrypter::decrypt(uint32_t print) {
	uint32_t huella = this->rsa(print, this->private_exponent, this->private_modulus);
	return this->encrypt_with_public_key(huella);
}