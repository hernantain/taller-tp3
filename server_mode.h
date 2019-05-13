#ifndef _SERVER_MODE
#define _SERVER_MODE

#include <iostream>

#include "common_certificate.h"
#include "common_key.h"
#include "common_socket.h"
#include "server_index.h"


class ServerMode {
	protected:
	Certificate new_cert;
	Socket skt;
	Key private_server_key;
	IndexHandler &index_handler;

	ServerMode(Socket &skt, 
			Key &private_server_key, 
			IndexHandler &index_handler);

	virtual void receive() = 0;
	virtual void process() = 0;
};


class ServerNewMode: public ServerMode {
	Key client_public_key;
	uint32_t calculated_print;

	bool user_present();

	void send();

	public:
	ServerNewMode(Socket &skt, 
				Key &private_server_key, 
				IndexHandler &index_handler);

	virtual void receive();

	virtual void process();
};

class ServerRevokeMode: public ServerMode {
	uint32_t encrypted_hash;

	void remove_user();

	public:
	ServerRevokeMode(Socket &skt, 
					Key &private_server_key, 
					IndexHandler &index_handler);

	virtual void receive();

	virtual void process();
};


#endif
