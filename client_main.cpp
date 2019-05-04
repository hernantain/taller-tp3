#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <arpa/inet.h>
#include <cstdint>
#include <string.h>
#include <ctime>

#include <vector>

#include "common_socket.h"
#include "common_key.h"
#include "common_certificate.h"
#include "client_info.h"
#include "common_encrypter.h"

#define NEW_COMMAND 0
#define REV_COMMAND 1
#define HOST_ARG 1
#define PORT_ARG 2
#define CLIENT_KEYS 4
#define PUB_SERV_KEYS 5
#define INFO_CERT 6
#define CLIENT_ARGS 7
#define MODE_ARG 3
#define NEW_MODE "new"
#define REV_MODE "revoke"

int main(int argc, char* argv[]) {
	if (argc != CLIENT_ARGS) {
		printf("Error: argumentos invalidos.\n");
		printf("Uso:\n./client <ip> <port> new/revoke <claves clientes> <publica servidor> <informacion certificado>\n");
		return 1;
	}

	std::string mode = argv[MODE_ARG];
	const char *host = argv[HOST_ARG];
	const char *port = argv[PORT_ARG];
	const char *client_keys = argv[CLIENT_KEYS];
	const char *pub_serv_keys = argv[PUB_SERV_KEYS];

	Socket skt(host, port);
	
	std::vector<Key*> keys = KeyFactory::Create(client_keys);
	Key public_client_key = *keys[0];
	Key private_client_key = *keys[1];
			
	Key server_pub_keys(pub_serv_keys);
	
	if (mode == NEW_MODE) {
		const char *req_info = argv[INFO_CERT];
		skt << (uint8_t) NEW_COMMAND;
		
		ClientInfo client_info(req_info);
		skt << client_info.get_name();;
		
		skt << public_client_key.get_modulus();
		skt << public_client_key.get_exponent();
		skt << client_info.get_start_date();
		skt << client_info.get_end_date();

		uint8_t status;
		skt >> status;
		if (status == 0) {
			std::cout << "Error: ya existe un certificado." << std::endl;
			return 1;
		} else {
			std::cout << "USUARIO NO REGISTRADO" << std::endl;
			Certificate certificate;
			skt >> certificate.serial_number;
			skt >> certificate.subject;
			//skt >> certificate.issuer;
			//std::cout << "Recibimos el issuer" << certificate.issuer << std::endl;
			skt >> certificate.start_date;
			skt >> certificate.end_date;
			skt >> certificate.client_modulus;
			skt >> certificate.client_exponent;

			std::cout << certificate();
			std::string cert_string = certificate();
			Encrypter encrypter(cert_string, private_client_key, server_pub_keys);

			uint32_t server_print;
			skt >> server_print;
			std::cout << "Huella del servidor: " << server_print << std::endl; 
			
			uint32_t server_hash = encrypter.decrypt(server_print);
			std::cout << "Hash del servidor: " << server_hash << std::endl; 

			encrypter.calculate_hash();
			uint32_t calculated_hash = encrypter.get_calculated_hash();
			std::cout << "Hash calculado: " << calculated_hash << std::endl;


			uint8_t hash_status = (calculated_hash == server_hash) ? 0 : 1;
			skt << hash_status;
			if (hash_status == 0){
				std::cout << "SON IGUALES!!!" << std::endl;
				certificate.save();
				return 0;
			} else {
				std::cout << "Error: los hashes no coiniciden." << std::endl;
				return 1;
			}
		}

	} else if (mode == REV_MODE) {
		skt << (uint8_t) REV_COMMAND;
		const char *certificate_file = argv[INFO_CERT];
		Certificate certificate(certificate_file);

		skt << certificate.serial_number;
		skt << certificate.subject;
		/////std::cout << "Por mandar el issuer: " << new_cert.issuer << std::endl;
		/////skt << certificate.issuer;
		skt << certificate.start_date;
		skt << certificate.end_date;
		skt << certificate.client_modulus;
		skt << certificate.client_exponent;

		std::string cert = certificate();

		Encrypter encrypter(cert, private_client_key, server_pub_keys);
		encrypter.calculate_hash();
		uint32_t encrypted_hash = encrypter.encrypt();

		skt << encrypted_hash;

		uint8_t revoke_status;
		skt >> revoke_status;
		if (revoke_status == 0) {
			std::cout << "TODO PIOLA" << std:: endl;
		} else if (revoke_status == 1) {
			std::cout << "Error: usuario no registrado." << std::endl;
			return 1;
		} else if (revoke_status == 2) {
			std::cout << "Error: los hashes no coiniciden." << std::endl;
			return 1;
		}

	} else {
		printf("Error: argumentos invalidos.\n"); 
		printf("Uso:\n./client <ip> <port> new/revoke <claves clientes> <publica servidor> <informacion certificado>\n");
		return 1;
	}


	return 0;
}

