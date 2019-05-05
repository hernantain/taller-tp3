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

#include "server_mode.h"

#define REGISTERED_CERTIFICATE 0
#define NEW_COMMAND 0
#define REV_COMMAND 1
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
	std::string index = argv[INDEX_ARG];

	Socket acep_skt(NULL, port);
	Socket connected_skt = acep_skt.accep();

	uint8_t command;
	connected_skt >> command;

	std::vector<Key*> keys = KeyFactory::Create(server_keys);
	//Key public_server_key = *keys[0];
	Key private_server_key = *keys[1];

	if (command == NEW_COMMAND) {
		printf("Hay que hacer un new...\n");
		
		ServerNewMode mode(connected_skt, private_server_key, index);
		
		mode.receive();

		mode.send();

	} else if (command == REV_COMMAND) {
		
		std::cout << "Es un revoke..." << std::endl;
			
		ServerRevokeMode mode(connected_skt, private_server_key, index);

		mode.receive();
	}
	
	return 0;
}
