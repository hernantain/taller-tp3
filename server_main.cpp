#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <vector>

#include "common_socket.h"
#include "common_key.h"
#include "common_certificate.h"
#include "server_users.h"
#include "common_encrypter.h"

#define REGISTERED_CERTIFICATE 0
#define NEW_COMMAND 0
#define NOT_REGISTERED_CERTIFICATE 1
#define PORT_ARG 1
#define SERVER_KEYS 2
#define INDEX_ARG 3
#define EXACT_ARGS 4

int main(int argc, char* argv[]) {

	if (argc != EXACT_ARGS) {
		printf("ERROR EN LA CANTIDAD DE ARGS\n");
		return 1;
	} 

	const char *port = argv[PORT_ARG];
	const char *server_keys = argv[SERVER_KEYS];
	const char *index = argv[INDEX_ARG];

	ActiveUsers active_users(index);

	Socket acep_skt(NULL, port);
	Socket connected_skt = acep_skt.accep();

	uint8_t command;
	connected_skt >> command;

	if (command == NEW_COMMAND) {
		printf("Hay que hacer un new...\n");
		
		Certificate new_cert;
		connected_skt >> new_cert.subject;
		connected_skt >> new_cert.client_modulus;
		connected_skt >> new_cert.client_exponent;
		connected_skt >> new_cert.start_date;
		connected_skt >> new_cert.end_date;
		Key client_public_key(new_cert.client_exponent, new_cert.client_modulus);

		if (active_users.has(new_cert.subject)) {
			connected_skt << (uint8_t) REGISTERED_CERTIFICATE;
		} else { 
			connected_skt << (uint8_t) NOT_REGISTERED_CERTIFICATE;

			new_cert.serial_number = (uint32_t) active_users.get_next_index();
			active_users.add(new_cert.subject, client_public_key);
			
			std::vector<Key*> keys = KeyFactory::Create(server_keys);
			//Key public_server_key = *keys[0];
			Key private_server_key = *keys[1];
			
			std::string cert_string = new_cert();
			//uint8_t server_exp = private_server_key.get_exponent();
			Encrypter encrypter(cert_string, private_server_key, client_public_key);
			encrypter.calculate_hash();
			std::cout << "Hash calculado: " << encrypter.get_calculated_hash() << std::endl;
			//std::cout << encrypter.encrypt() << std::endl;
			std::cout << cert_string; 

			connected_skt << new_cert.serial_number;
			connected_skt << new_cert.subject;
			/////std::cout << "Por mandar el issuer: " << new_cert.issuer << std::endl;
			/////connected_skt << new_cert.issuer;
			connected_skt << new_cert.start_date;
			connected_skt << new_cert.end_date;
			connected_skt << new_cert.client_modulus;
			connected_skt << new_cert.client_exponent;
			uint32_t calculated_print = encrypter.encrypt();
			connected_skt << calculated_print;
			std::cout << "Huella calculada: " << calculated_print << std::endl;
			uint8_t hashing_status;
			connected_skt >> hashing_status;
			
			if (hashing_status == 1) {
				std::cout << "Error" << std::endl;
				return 1;
			}

			active_users.save();
		}

	} else {

	}

	//printf("%s\n", buf);

	return 0;
}
