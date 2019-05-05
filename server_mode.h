#ifndef _SERVER_MODE
#define _SERVER_MODE

#include <iostream>

#include "common_certificate.h"
#include "common_key.h"
#include "common_socket.h"
#include "server_users.h"


class ServerMode {
	
	protected:
	Certificate new_cert;
	Socket skt;
	Key private_server_key;
	ActiveUsers active_users;

	ServerMode(Socket &skt, Key &private_server_key, std::string &index);

	virtual void receive() = 0;

	public:
	void send();
};


class ServerNewMode: public ServerMode {

	Key client_public_key;

	public:

	ServerNewMode(Socket &skt, Key &private_server_key, std::string &index);

	virtual void receive();

};

class ServerRevokeMode: public ServerMode {

	public:

	ServerRevokeMode(Socket &skt, Key &private_server_key, std::string &index);

	virtual void receive();

};


#endif