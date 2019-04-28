#include <fstream>
#include <cstdint>

#include "common_key.h"

Key::Key(const char *file_name) {
	this->file.open(file_name);
	uint16_t a, b, c;
	this->file >> a >> b >> c;
	this->public_key = (uint8_t) a; 
	this->private_key = (uint8_t) b;
	this->mod = c;
}

uint8_t Key::get_public_key() {
	return this->public_key;
}
uint8_t Key::get_private_key() {
	return this->private_key;
}

uint16_t Key::get_modulus() {
	return this->mod;
}

Key::~Key() {
	this->file.close();
}