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


class ClientThread: public Thread {
	Socket connected_skt;
	IndexHandler &index_handler;

	public:
	ClientThread(Socket connected_skt, IndexHandler &index_handler);

	virtual void stop();

	virtual void run();
};



class AcceptorThread: public Thread {
	bool must_run;
	const char *port, *server_keys;
	std::string index;
	std::vector<Thread*> clients;

	public:
	AcceptorThread(const char *port, const char *server_keys, std::string &index);

	virtual void stop();

	virtual void run();

	~AcceptorThread();
};




#endif

