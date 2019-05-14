#ifndef _CLIENT_MODE
#define _CLIENT_MODE

#include <iostream>
#include <string>

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
	
	ClientMode(Socket &skt, 
			Key &public_client_key, 
			Key &private_client_key, 
			Key &server_pub_keys);

	virtual void send() = 0;
	virtual void process() = 0;
};


class ClientNewMode: public ClientMode {
	Certificate certificate;
	ClientInfo client_info;
	std::string cert_string;

	virtual void send();
	bool user_present();
	void receive();
	
	public:
	ClientNewMode(Socket &skt, 
			Key &public_client_key, 
			Key &private_client_key, 
			Key &server_pub_keys, 
			std::string &client_req);

	virtual void process();
};


class ClientRevokeMode: public ClientMode {
	Certificate certificate;
	std::string cert;
	
	virtual void send();
	void print_revoke_status();

	public:
	ClientRevokeMode(Socket &skt, 
				Key &public_client_key, 
				Key &private_client_key, 
				Key &server_pub_keys, 
				std::string &certificate_file);

	virtual void process();
};

#endif
