#ifndef _KEY
#define _KEY

#include <cstdint>
#include <fstream>

class Key {
	
	uint8_t public_key, private_key;
	uint16_t mod;
	std::ifstream file;

	public:
	
	Key(const char *file_name);

	uint8_t get_public_key();

	uint8_t get_private_key();

	uint16_t get_modulus();

	~Key();
};

#endif
