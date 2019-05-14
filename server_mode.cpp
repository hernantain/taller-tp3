#include <iostream>
#include <string>

#include "common_certificate.h"
#include "common_key.h"
#include "common_socket.h"
#include "common_encrypter.h"
#include "server_mode.h"

#define REGISTERED_CERTIFICATE 0
#define VALIDATION_OK 0
#define INVALID_HASH 1
#define NOT_REGISTERED_CERTIFICATE 1
#define VALIDATION_FAILED 2


ServerMode::ServerMode(Socket &skt, 
					Key &private_server_key, 
					IndexHandler &index_handler) : skt(skt),
									private_server_key(private_server_key),
									index_handler(index_handler) {}


ServerNewMode::ServerNewMode(Socket &skt, 
				Key &private_server_key, 
				IndexHandler &index_handler) : 
				ServerMode(skt, 
						private_server_key,
						index_handler) {}


void ServerNewMode::process() {
	this->receive();
	
	if (!this->user_present())
		this->send();
}


void ServerNewMode::receive() {
	this->skt >> this->new_cert.subject;
	this->skt >> this->new_cert.client_modulus;
	this->skt >> this->new_cert.client_exponent;
	this->skt >> this->new_cert.start_date;
	this->skt >> this->new_cert.end_date;
}


bool ServerNewMode::user_present() {
	if (this->index_handler.has(this->new_cert.subject)) {
		this->skt << (uint8_t) REGISTERED_CERTIFICATE;
		return true;
	} else { 
		this->skt << (uint8_t) NOT_REGISTERED_CERTIFICATE;

		this->new_cert.serial_number = (uint32_t) 
									this->index_handler.get_next_index();
		
		Key client_public_key(this->new_cert.client_exponent, 
							this->new_cert.client_modulus);
		
		this->index_handler.add(this->new_cert.subject, client_public_key);
				
		std::string cert_string = this->new_cert();
		Encrypter encrypter(cert_string, 
							this->private_server_key, 
							client_public_key);

		encrypter.calculate_hash();
		this->calculated_print = encrypter.encrypt();
		return false;
	}
}


void ServerNewMode::send() {
	this->skt << this->new_cert.serial_number;
	this->skt << this->new_cert.subject;
	this->skt << this->new_cert.issuer;
	this->skt << this->new_cert.start_date;
	this->skt << this->new_cert.end_date;
	this->skt << this->new_cert.client_modulus;
	this->skt << this->new_cert.client_exponent;	
	this->skt << this->calculated_print;
		
	uint8_t hashing_status;
	this->skt >> hashing_status;
	if (hashing_status == INVALID_HASH) {
		this->index_handler.remove(this->new_cert.subject);
	}
}


ServerRevokeMode::ServerRevokeMode(Socket &skt, 
					Key &private_server_key, 
					IndexHandler &index_handler) : ServerMode(skt, 
									private_server_key, 
									index_handler) {}


void ServerRevokeMode::process() {
	this->receive();
	this->remove_user();
}


void ServerRevokeMode::receive() {
	this->skt >> this->new_cert.serial_number;
	this->skt >> this->new_cert.subject;
	this->skt >> this->new_cert.issuer;
	this->skt >> this->new_cert.start_date;
	this->skt >> this->new_cert.end_date;
	this->skt >> this->new_cert.client_modulus;
	this->skt >> this->new_cert.client_exponent;
	this->skt >> this->encrypted_hash;
}


void ServerRevokeMode::remove_user() {
	if (this->index_handler.has(this->new_cert.subject)) {
		Key client_pub_key = this->index_handler.get_key(this->new_cert.subject);
		std::string stringCert = this->new_cert();

		Encrypter encrypter(stringCert, 
						this->private_server_key,
						client_pub_key);
		uint32_t decrypted_hash = encrypter.decrypt(this->encrypted_hash);

		encrypter.calculate_hash();
		uint32_t calculated_hash = encrypter.get_calculated_hash();

		uint8_t hash_status = (calculated_hash == decrypted_hash) ? 
								VALIDATION_OK : VALIDATION_FAILED;

		if (hash_status == VALIDATION_OK) {
			this->index_handler.remove(this->new_cert.subject);
		}
		this->skt << hash_status;
	} else {
		this->skt << (uint8_t) NOT_REGISTERED_CERTIFICATE;
	}
}
