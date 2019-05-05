#ifndef _CLIENT_MODE
#define _CLIENT_MODE

#include <iostream>

#include "common_socket.h"
#include "common_key.h"
#include "common_certificate.h"
#include "common_encrypter.h"
#include "common_certificate.h"
#include "client_info.h"

class ClientMode {

	protected:
	Socket skt;
	Key public_client_key, private_client_key, server_pub_keys;
	
	ClientMode(Socket &skt, Key &public_client_key, Key &private_client_key, 
					Key &server_pub_keys);


	virtual void send() = 0;

	public:
	void receive();

};


class ClientNewMode: public ClientMode {

	ClientInfo client_info;

	public:
	ClientNewMode(Socket &skt, Key &public_client_key, Key &private_client_key, 
				Key &server_pub_keys, std::string &client_req);


	virtual void send();
};


class ClientRevokeMode: public ClientMode {

	Certificate certificate;

	public:
	ClientRevokeMode(Socket &skt, Key &public_client_key, Key &private_client_key, 
				Key &server_pub_keys, std::string &certificate_file);

	virtual void send();
};

#endif
