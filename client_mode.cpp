#include <iostream>

#include "common_socket.h"
#include "common_key.h"
#include "common_certificate.h"
#include "common_encrypter.h"
#include "client_info.h"

#include "client_mode.h"


ClientMode::ClientMode(Socket &skt, Key &public_client_key, Key &private_client_key, 
					Key &server_pub_keys) : skt(skt),
										public_client_key(public_client_key),
										private_client_key(private_client_key),
										server_pub_keys(server_pub_keys) {}

ClientNewMode::ClientNewMode(Socket &skt,
				Key &public_client_key, 
				Key &private_client_key, 
				Key &server_pub_keys, 
				std::string &client_req) : ClientMode(skt, 
								public_client_key,
								private_client_key, 
								server_pub_keys), client_info(client_req) {}


void ClientNewMode::send() {
	this->skt << this->client_info.get_name();
	this->skt << this->public_client_key.get_modulus();
	this->skt << this->public_client_key.get_exponent();
	this->skt << this->client_info.get_start_date();
	this->skt << this->client_info.get_end_date();
}

void ClientMode::receive() {
	uint8_t status;
	skt >> status;
	if (status == 0) {
		std::cout << "Error: ya existe un certificado." << std::endl;
		exit(1);
	} else {
		Certificate certificate;
		skt >> certificate.serial_number;
		skt >> certificate.subject;
		skt >> certificate.issuer;
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
			exit(0);
		} else {
			std::cout << "Error: los hashes no coiniciden." << std::endl;
			exit(1);
		}
	}
}


ClientRevokeMode::ClientRevokeMode(Socket &skt,
				Key &public_client_key, 
				Key &private_client_key, 
				Key &server_pub_keys, 
				std::string &certificate_file) : ClientMode(skt, 
								public_client_key,
								private_client_key, 
								server_pub_keys), 
								certificate(certificate_file) {}


void ClientRevokeMode::send() {
	this->skt << this->certificate.serial_number;
	this->skt << this->certificate.subject;
	this->skt << this->certificate.issuer;
	this->skt << this->certificate.start_date;
	this->skt << this->certificate.end_date;
	this->skt << this->certificate.client_modulus;
	this->skt << this->certificate.client_exponent;

	std::string cert = this->certificate();

	Encrypter encrypter(cert, private_client_key, server_pub_keys);
	encrypter.calculate_hash();
	uint32_t encrypted_hash = encrypter.encrypt();

	this->skt << encrypted_hash;

	uint8_t revoke_status;
	this->skt >> revoke_status;
	if (revoke_status == 0) {
		std::cout << "TODO PIOLA" << std:: endl;
	} else if (revoke_status == 1) {
		std::cout << "Error: usuario no registrado." << std::endl;
		exit(1);
	} else if (revoke_status == 2) {
		std::cout << "Error: los hashes no coiniciden." << std::endl;
		exit(1);
	}
}
