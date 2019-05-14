#ifndef _SERVER_THREAD
#define _SERVER_THREAD

#include <iostream>
#include <vector>
#include <thread>
#include <string>

#include "common_socket.h"
#include "server_index.h"
#include "server_mode.h"


#define NEW_COMMAND 0
#define REV_COMMAND 1

//Clase Thread: encapsulta el thread correspondiente y 
//tiene los metodos start y join que llamaran y cerraran 
//al thread que corresponda con el metodo que corresponda.
class Thread {
	private:
		std::thread thread;
 
	public:
		Thread();

		void start();
		void join();

		virtual void run() = 0; 
		virtual void stop() = 0;
		
		virtual ~Thread();

		Thread(const Thread&) = delete;
		Thread& operator=(const Thread&) = delete;

		Thread(Thread&& other);


		Thread& operator=(Thread&& other);
};


//Hilo que correra las comunicaciones correspondientes con el cliente.
//Puede haber multiples hilos de este tipo corriendo a la vez
class ClientThread: public Thread {
	Socket connected_skt;
	IndexHandler &index_handler;
	Key &private_server_key;

	public:
	ClientThread(Socket connected_skt, 
			IndexHandler &index_handler, 
			Key &private_server_key);

	virtual void stop();

	virtual void run();
};


//Hilo que acepta conexiones y llama a ClientThread con la informacion
//correspondiente.
//Correra hasta que en otro hilo (main) se lea una 'q' de stdin y este 
//llame al metodo stop, deteniendo asi el thread.
class AcceptorThread: public Thread {
	bool must_run;
	const char *port, *server_keys;
	std::string index;
	std::vector<Thread*> clients;
	Key private_server_key;

	public:
	AcceptorThread(const char *port, const char *server_keys, std::string &index);

	virtual void stop();

	virtual void run();

	~AcceptorThread();
};

#endif

