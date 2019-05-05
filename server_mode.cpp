#include <iostream>

#include "common_certificate.h"
#include "common_key.h"
#include "common_socket.h"
#include "common_encrypter.h"

#include "server_mode.h"

#define REGISTERED_CERTIFICATE 0
#define NOT_REGISTERED_CERTIFICATE 1



ServerMode::ServerMode(Socket &skt, 
					Key &private_server_key, 
					std::string &index) : skt(skt),
										private_server_key(private_server_key),
										active_users(index) {}


ServerNewMode::ServerNewMode(Socket &skt, 
					Key &private_server_key, 
					std::string &index) : ServerMode(skt, private_server_key, index) {}



void ServerNewMode::receive() {
	this->skt >> this->new_cert.subject;
	this->skt >> this->new_cert.client_modulus;
	this->skt >> this->new_cert.client_exponent;
	this->skt >> this->new_cert.start_date;
	this->skt >> this->new_cert.end_date;
}

void ServerMode::send() {
	Key client_public_key(this->new_cert.client_exponent, this->new_cert.client_modulus);

	if (this->active_users.has(this->new_cert.subject)) {
		this->skt << (uint8_t) REGISTERED_CERTIFICATE;
	} else { 
		this->skt << (uint8_t) NOT_REGISTERED_CERTIFICATE;

		this->new_cert.serial_number = (uint32_t) this->active_users.get_next_index();
		this->active_users.add(this->new_cert.subject, client_public_key);
		
		
		std::string cert_string = this->new_cert();
		Encrypter encrypter(cert_string, this->private_server_key, client_public_key);
		encrypter.calculate_hash();
		std::cout << "Hash calculado: " << encrypter.get_calculated_hash() << std::endl;
		std::cout << cert_string; 

		this->skt << this->new_cert.serial_number;
		this->skt << this->new_cert.subject;
		this->skt << this->new_cert.issuer;
		this->skt << this->new_cert.start_date;
		this->skt << this->new_cert.end_date;
		this->skt << this->new_cert.client_modulus;
		this->skt << this->new_cert.client_exponent;
		
		uint32_t calculated_print = encrypter.encrypt();
		this->skt << calculated_print;
			
		uint8_t hashing_status;
		this->skt >> hashing_status;
		if (hashing_status == 1) {
			std::cout << "Error, los hashes no coincidieron" << std::endl;
			exit(1);
		}

		this->active_users.save();
	}
}


ServerRevokeMode::ServerRevokeMode(Socket &skt, 
					Key &private_server_key, 
					std::string &index) : ServerMode(skt, 
									private_server_key, 
									index) {}


void ServerRevokeMode::receive() {
	this->skt >> this->new_cert.serial_number;
	this->skt >> this->new_cert.subject;
	this->skt >> this->new_cert.issuer;
	this->skt >> this->new_cert.start_date;
	this->skt >> this->new_cert.end_date;
	this->skt >> this->new_cert.client_modulus;
	this->skt >> this->new_cert.client_exponent;

	uint32_t encrypted_hash;
	this->skt >> encrypted_hash;

	if (this->active_users.has(this->new_cert.subject)) {
		std::cout << this->new_cert.subject << " esta en los registros" << std::endl;
		
		Key client_pub_key = this->active_users.get_key(this->new_cert.subject);
		std::string stringCert = this->new_cert();

		Encrypter encrypter(stringCert, this->private_server_key, client_pub_key);
		uint32_t decrypted_hash = encrypter.decrypt(encrypted_hash);

		encrypter.calculate_hash();
		uint32_t calculated_hash = encrypter.get_calculated_hash();

		uint8_t hash_status = (calculated_hash == decrypted_hash) ? 0 : 2;
		if (hash_status == 0) {
			this->active_users.remove(this->new_cert.subject);
			std::cout << "Eliminando del map a: " << this->new_cert.subject << std::endl;
			this->active_users.save();
		}
		this->skt << hash_status;
	} else {
		this->skt << (uint8_t) NOT_REGISTERED_CERTIFICATE;
	}
}