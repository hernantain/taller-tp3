#ifndef _KEY
#define _KEY

#include <cstdint>
#include <fstream>
#include <vector>

//Almacena la informacion de las keys. 
//Puede ser creada pasandole el exponente y el modulo
//o el archivo correspondiente para que lea las claves.
class Key {
	uint8_t exponent;
	uint16_t mod;

	public:
	Key(uint8_t exponent, uint16_t mod);

	explicit Key(const char *file_name);

	Key();

	uint8_t get_exponent();
	
	uint16_t get_modulus();
};


//En el caso de que un archivo contenga la clave privada y publica
//del cliente o del server, se le puede pasar al metodo estatico 'Create'
//el archivo por parametro y esta clase devlvera un vector en el que la 
//primer posicion sera la clave publica y la segunda la clave privada.
class KeyFactory {
	public:
	static std::vector<Key> Create(const char *file_name);
};

#endif
