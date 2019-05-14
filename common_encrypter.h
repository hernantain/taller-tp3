#ifndef _SERVER_ENCRYPTER
#define _SERVER_ENCRYPTER

#include <string>
#include <sstream>
#include <cstdint>

#include "common_key.h"

//Clase encargada de encriptar y desencriptar el certificado.
class Encrypter {
	uint8_t private_exponent, public_exponent;
	uint16_t private_modulus, public_modulus;
	uint32_t hash_calculated, hash_encrypted_private;
	std::string certificate;

	//Aplica el algoritmo de encriptacion al numero recibido con 
	//el exponente y el modulo recibidos por paramteros
	uint32_t rsa(uint32_t num, uint8_t exponent, uint16_t mod);

	//Llama a rsa con la clave privada con la que haya sido creado 
	//el encrypter.
	uint32_t encrypt_with_private_key();
	
	//Llama a rsa con la clave publica con la que haya sido creado 
	//el encrypter.
	uint32_t encrypt_with_public_key(uint32_t &tmp_huella);

	public:
	Encrypter(std::string &cert, Key &private_key, Key &public_key);

	//Calcula el hash del certificado
	void calculate_hash();

	uint32_t get_calculated_hash();

	uint32_t get_hash_encrypted_with_private();

	//Llama a los meotods de encriptacion en el orden que corresponda.
	uint32_t encrypt();

	//Desencripta la huella pasada por paremtro.
	uint32_t decrypt(uint32_t print);
};

#endif
