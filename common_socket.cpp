#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "common_socket.h"

#define BYTE_LEN 1
#define TWO_BYTE_LEN 2
#define FOUR_BYTE_LEN 4
#define MAX_LSTN_CON 10

Socket::Socket(const char *hostname, const char *port) {
	struct addrinfo hints, *results, *rp;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = (!hostname) ? AI_PASSIVE : 0;

	int status = getaddrinfo(hostname, port, &hints, &results);

	if (status != 0){
		perror("getaddrinfo");
		exit(1);
	}

	for (rp = results; rp != NULL; rp = rp->ai_next) {
		this->fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (this->fd == -1) {
			printf("Error: %s\n", strerror(errno));
			continue;
		} 

		if (!hostname) {
			if ( bind(this->fd, results->ai_addr, results->ai_addrlen) < 0 ) {
				perror("bind");
				printf("%sn",strerror(errno));
				freeaddrinfo(results);
				exit(1);
			}

			if ( listen(this->fd, MAX_LSTN_CON) == -1 ) {
				perror("listen");
				printf("%s\n", strerror(errno));
				exit(1);
			}
		} else {
			if (connect(this->fd, rp->ai_addr, rp->ai_addrlen) != -1)
				break;

			close(this->fd); 
		}

	}
	freeaddrinfo(results);
}

Socket::Socket(int fd) {
	this->fd = fd;
}

Socket Socket::accep() {
	int fd = accept(this->fd, NULL, NULL);
	return std::move(Socket(fd));
}

void Socket::receive(char *buf, int size) {
	int received = 0;
	int s = 0;
	while (received < size) {
		s = recv(this->fd, &buf[received], size-received, MSG_NOSIGNAL);
		if (s <= 0) { 
			return;
		} else {
			received += s;
		}
	}
	return;
}


void Socket::send_message(char *buf, int size) {
	int sent = 0;
	int s = 0;

	while (sent < size) {
		s = send(this->fd, &buf[sent], size-sent, MSG_NOSIGNAL);
		if (s <= 0) {
			return;
		} else {
			sent += s;
		}
	}
	return;
}

void Socket::operator<<(uint8_t command) {
	this->send_message((char*) &command, BYTE_LEN);
}

void Socket::operator<<(uint16_t mod) {
	this->send_message((char*) &mod, TWO_BYTE_LEN);	
}

void Socket::operator<<(uint32_t num) {
	this->send_message((char*) &num, FOUR_BYTE_LEN);	
}

void Socket::operator<<(std::string message) {
	uint32_t len = htonl(message.size());
	this->send_message( (char*) &len, FOUR_BYTE_LEN);
	this->send_message( (char*) message.c_str(), (int) message.length());	
}

void Socket::operator>>(uint8_t &buf) {
	this->receive((char*) &buf, BYTE_LEN);
}


void Socket::operator>>(uint16_t &buf) {
	this->receive((char*) &buf, TWO_BYTE_LEN);
}


void Socket::operator>>(uint32_t &buf) {
	this->receive((char*) &buf, FOUR_BYTE_LEN);
}


void Socket::operator>>(std::string &s) {
	uint32_t len;
	this->receive((char*) &len, FOUR_BYTE_LEN);
	len = ntohl(len);
	char *tmp = (char*) calloc( len, sizeof(char));
	this->receive(tmp, len);
	s = tmp;
	free(tmp);
}
