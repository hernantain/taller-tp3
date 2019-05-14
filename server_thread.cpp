#include <iostream>
#include <vector>
#include <thread>
#include <unistd.h>
#include <string>

#include "common_socket.h"
#include "server_index.h"
#include "server_thread.h"


Thread::Thread() {}

void Thread::start() {
	thread = std::thread(&Thread::run, this);
}

void Thread::join() {
	thread.join();
}

Thread::~Thread() {} 

Thread::Thread(Thread&& other) {
	this->thread = std::move(other.thread);
}


Thread& Thread::operator=(Thread&& other) {
	this->thread = std::move(other.thread);
	return *this;
}


ClientThread::ClientThread(Socket connected_skt, 
							IndexHandler &index_handler,
							Key &private_server_key) : 
							connected_skt(connected_skt),
							index_handler(index_handler),
							private_server_key(private_server_key) {}

void ClientThread::stop() {}

void ClientThread::run() {
	uint8_t command;
	this->connected_skt >> command;

	if (command == NEW_COMMAND) {
		ServerNewMode mode(this->connected_skt, 
						this->private_server_key,
						this->index_handler);
		
	
		mode.process();
	} else if (command == REV_COMMAND) {
		ServerRevokeMode mode(this->connected_skt, 
							this->private_server_key, 
							this->index_handler);
	
		mode.process();
	}
}


AcceptorThread::AcceptorThread(const char *port, 
							const char *server_keys, 
							std::string &index) {
		std::vector<Key> keys = KeyFactory::Create(server_keys);
		this->private_server_key = keys[1];
		this->port = port;
		this->must_run = true;
		this->index = index;
	}

void AcceptorThread::stop() {
	this->must_run = false;
}

void AcceptorThread::run() {
	Socket acep_skt(NULL, this->port, true); 
	IndexHandler index_handler(this->index);
	while (true) {
		Socket connected_skt = acep_skt.accep();
		sleep(1);
		if (!this->must_run)
			break;
		
		if (connected_skt.is_valid) {
			Thread *ct = new ClientThread(connected_skt, 
										index_handler, 
										this->private_server_key);
			ct->start();
			this->clients.push_back(ct);
		}
	}
	index_handler.save();
}


AcceptorThread::~AcceptorThread() {
	for (size_t i = 0; i < this->clients.size(); ++i) {
		this->clients[i]->join();
		delete this->clients[i];
	}
}
