#ifndef _COMMON_SOCKET
#define _COMMON_SOCKET


class Socket {
	int fd; 

	Socket(int fd, bool valid);

	public:

	bool is_valid;

	Socket(const char *hostname, const char *port, bool server);

	Socket accep();

	void receive(char *buf, int size);

	void send_message(char *buf, int size);

	void operator<<(uint8_t command);

	void operator<<(uint16_t mod);

	void operator<<(uint32_t num);

	void operator<<(std::string mes);

	void operator>>(uint8_t &buf);

	void operator>>(uint16_t &buf);

	void operator>>(uint32_t &buf);

	void operator>>(std::string &s);
};

#endif
