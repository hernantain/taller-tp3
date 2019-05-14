#ifndef _SERVER_USERS
#define _SERVER_USERS

#include <fstream>
#include <map>
#include <iostream>
#include <string>
#include <mutex>

#include "common_key.h"

//Clase que maneja el archivo index del server.
//Contiene un map donde la key es el string sel subject del 
//certificado y el valor es un objeto key.
class IndexHandler {
	int current_index;
	std::string file_name;
	std::ifstream read_file;
	std::ofstream write_file;
	std::map<std::string, Key> private_map;
	std::mutex m;

	public:
	explicit IndexHandler(std::string &file_name);

	//Se fija en tiempo logN si el subject ya se encuentra en el map o no.
	bool has(std::string key);

	//Añade un subject con su objeto key correspondiente al map.
	void add(std::string user, Key &key);

	//Devuelve el indice correspondiente para el certificado a crear.
	int get_next_index();

	//Devuelve el objeto key para el user correspondiente.
	Key get_key(std::string user);

	//Borra del map el user corresponiente.
	void remove(std::string user);

	//Guarda el archivo index.
	void save();
};

#endif
