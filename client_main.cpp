#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <arpa/inet.h>
#include <cstdint>
#include <string.h>
#include <ctime>

#include "common_socket.h"
#include "common_key.h"
#include "common_certificate.h"
#include "client_info.h"
#include "common_encrypter.h"

#define NEW_COMMAND 0
#define HOST_ARG 1
#define PORT_ARG 2
#define CLIENT_ARGS 7
#define MODE_ARG 3
#define NEW_MODE "new"
#define REV_MODE "revoke"

/*uint32_t rsa(uint32_t num, uint8_t exponent, uint16_t mod) {
	unsigned char byte = (char) num;
	uint32_t sum = 0;

	for (int i = 0; i < 4; ++i) {
		int res = 1;
		for (unsigned char j = 1; j < exponent; ++j) {
			res = (res*byte) % mod;
		}
		byte = num >> 8;
		(i != 3) ? sum += res : sum += res*256;
	}

	return sum;
}*/


int main(int argc, char* argv[]) {


	//CHEQUEO CANTIDAD DE ARGS.
	
	/*if (argc != CLIENT_ARGS) {
		printf("Error: argumentos invalidos.\n");
		printf("Uso:\n./client <ip> <port> new/revoke <claves clientes> <publica servidor> <informacion certificado>\n");
		return 1;
	}

	std::string mode = argv[MODE_ARG];
	const char *host = argv[HOST_ARG];
	const char *port = argv[PORT_ARG];

	if (mode == NEW_MODE) {
		Socket skt(host, port);
		skt << (uint8_t) NEW_COMMAND;
		
		ClientInfo client_info("req_info.txt");
		skt << client_info.get_name();

		Key client_keys("client.keys");
	
		skt << client_keys.get_modulus();
		skt << client_keys.get_public_key();
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
			uint8_t cli_pri = client_keys.get_private_key();
			uint8_t a = 13;
			Encrypter encrypter(cert_string, a, cli_pri);


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
				return 0;
			} else {
				std::cout << "Error: los hashes no coiniciden." << std::endl;
				return 1;
			}
		}

	} else if (mode == REV_MODE) {

	} else {
		printf("Error: argumentos invalidos.\n"); 
		printf("Uso:\n./client <ip> <port> new/revoke <claves clientes> <publica servidor> <informacion certificado>\n");
		return 1;
	}*/

	return 0;
}


/*
std::tm my_t;
memset(&my_t, 0, sizeof(std::tm));
const char *date = "Apr 26 20:34:10 2019";
strptime(date, "%b %d %H:%M:%S %Y", &my_t);
char buf[21];

strftime(buf, sizeof(buf), "%b %d %H:%M:%S %Y", &my_t);


printf("%s\n", buf);*/