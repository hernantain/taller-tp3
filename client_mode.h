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

//CLASES PARA MANEJAR LOS DISTINTOS TIPOS DE REQUEST


//Clase Base. Almacena el socket y todas las keys que tienen en comun
//los modos.
class ClientMode {
	protected:
	Socket skt;
	Key &public_client_key, &private_client_key, &server_pub_keys;
	
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

	//Envia el archivo en el orden descripto por el enunciado 
	//para el modo new.
	virtual void send();

	//Recibe del server un byte sin signo indicando
	//si el user tiene un certificado vigente o no.
	//Si se encuentra vigente imprime por pantalla. 
	//Caso contrario recibe el certificado en el 
	//orden Correspondiente.
	bool user_present();

	//Recibe la huella y el hash del servidor, desencripta
	//Calcula el hash del certificado recibido e imprime por pantalla
	//el resultado.
	void print_new_status();

	//Recibe el certificado de parte del servidor.
	void receive();
	
	public:
	ClientNewMode(Socket &skt, 
			Key &public_client_key, 
			Key &private_client_key, 
			Key &server_pub_keys, 
			std::string &client_req);

	//Llama a los metodos de los distintos modos. 
	virtual void process();
};


class ClientRevokeMode: public ClientMode {
	Certificate certificate;
	std::string cert;
	
	//Envia el archivo en el orden descripto por el enunciado 
	//para el modo revoke.
	virtual void send();

	//Encripta el certificado que envio previamente
	//Imprime por pantalla: 
	//	-Hash calculado
	//	-Hash encriptado con clave privada
	//	-Huella enviada
	//O los distintos tipos de error que pueden existir para este modo.
	void print_revoke_status();

	public:
	ClientRevokeMode(Socket &skt, 
				Key &public_client_key, 
				Key &private_client_key, 
				Key &server_pub_keys, 
				std::string &certificate_file);

	//Llama a los metodos de los distintos modos.
	virtual void process();
};

#endif
