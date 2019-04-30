#ifndef _KEY
#define _KEY

#include <cstdint>
#include <fstream>
#include <vector>

class Key {

	uint8_t exponent;
	uint16_t mod;

	public:
	Key(uint8_t exponent, uint16_t mod);

	Key(const char *file_name);

	Key();

	uint8_t get_exponent();
	
	uint16_t get_modulus();

};


class KeyFactory {
	public:
	static std::vector<Key*> Create(const char *file_name);
};

#endif
