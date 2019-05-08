#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <thread>

#include "common_socket.h"
#include "common_key.h"
#include "common_certificate.h"
#include "common_encrypter.h"
#include "server_thread.h"
#include "server_index.h"

#define REGISTERED_CERTIFICATE 0
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

	Thread *acceptor = new AcceptorThread(port, server_keys, index);

	acceptor->start();

	char c;
	do {
		c = getchar();
		std::cout << c;
	} while (c != 'q');

	acceptor->stop();
	acceptor->join();
	delete acceptor;

	return 0;
}
