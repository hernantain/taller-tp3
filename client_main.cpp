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
#include "client_mode.h"

#define NEW_COMMAND 0
#define REV_COMMAND 1
#define HOST_ARG 1
#define PORT_ARG 2
#define CLIENT_KEYS 5
#define PUB_SERV_KEYS 6
#define INFO_CERT 4
#define CLIENT_ARGS 7
#define MODE_ARG 3
#define NEW_MODE "new"
#define REV_MODE "revoke"

int main(int argc, char* argv[]) {
	if (argc != CLIENT_ARGS) {
		printf("Error: argumentos invalidos.\n");
		return 0;
	}

	std::string mode = argv[MODE_ARG];
	const char *host = argv[HOST_ARG];
	const char *port = argv[PORT_ARG];
	const char *client_keys = argv[CLIENT_KEYS];
	const char *pub_serv_keys = argv[PUB_SERV_KEYS];

	Socket skt(host, port, false);
	
	std::vector<Key> keys = KeyFactory::Create(client_keys);
	Key public_client_key = keys[0];
	Key private_client_key = keys[1];
			
	Key server_pub_keys(pub_serv_keys);
	
	if (mode == NEW_MODE) {
		std::string req_info = argv[INFO_CERT];
		skt << (uint8_t) NEW_COMMAND;
		
		ClientNewMode mode(skt, 
						public_client_key, 
						private_client_key, 
						server_pub_keys, 
						req_info);

		mode.process();
		//mode.send();
		//mode.receive();
	} else if (mode == REV_MODE) {
		skt << (uint8_t) REV_COMMAND;
		std::string certificate_file = argv[INFO_CERT];

		ClientRevokeMode mode(skt, 
							public_client_key, 
							private_client_key, 
							server_pub_keys, 
							certificate_file);

		mode.process();
		//mode.send();
	} else {
		printf("Error: argumentos invalidos.\n"); 
		return 0;
	}

	return 0;
}
