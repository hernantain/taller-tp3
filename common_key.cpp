#include <fstream>
#include <cstdint>
#include <vector>
#include <iostream>

#include "common_key.h"

std::vector<Key*> KeyFactory::Create(const char *file_name) {
	std::vector<Key*> keys;
	std::ifstream file;
	file.open(file_name);
	uint16_t public_exp, private_exp, mod;
	file >> public_exp >> private_exp >> mod;
	Key public_key((uint8_t) public_exp, mod);
	keys.push_back(&public_key);
	Key private_key((uint8_t) private_exp, mod);
	keys.push_back(&private_key);
	return keys;
}

Key::Key(){}

Key::Key(uint8_t exponent, uint16_t mod) {
	this->exponent = exponent;
	this->mod = mod;
}

Key::Key(const char *file_name) {
	std::ifstream file;
	file.open(file_name);
	uint16_t exponent, mod;
	file >> exponent >> mod;
	this->exponent = (uint8_t) exponent;
	this->mod = mod;
}

uint8_t Key::get_exponent() {
	return this->exponent;
}

uint16_t Key::get_modulus() {
	return this->mod;
}
