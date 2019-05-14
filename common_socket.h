#ifndef _COMMON_SOCKET
#define _COMMON_SOCKET

#include <string>

//Clase que encapsula toda la logica de sockets.
class Socket {
	int fd; 

	Socket(int fd, bool valid);

	public:
	bool is_valid;

	//Construye un socket aceptador o para el cliente segun los parametros
	//que le san pasados. 
	Socket(const char *hostname, const char *port, bool server);

	//Metodo que acepta una conexion y devuelve un nuevo socket
	//que se comunicara con el cliente
	Socket accep();

	//Metodo usado para enviar a traves del socket.
	void receive(char *buf, int size);

	//Metodo usado para recibir a traves del socket.
	void send_message(char *buf, int size);

	//Operadores para enviar a traves del socket sobrecargados para 
	//distintos tipos. Llaman a send_message 
	//con los parametros correspondientes.
	void operator<<(uint8_t command);

	void operator<<(uint16_t mod);

	void operator<<(uint32_t num);

	void operator<<(std::string mes);

	//Operadores para recibir a traves del socket sobrecargados para 
	//distintos tipos. Llaman a receive
	//con los parametros correspondientes.
	void operator>>(uint8_t &buf);

	void operator>>(uint16_t &buf);

	void operator>>(uint32_t &buf);

	void operator>>(std::string &s);
};

#endif
