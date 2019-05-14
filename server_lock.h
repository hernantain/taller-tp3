#ifndef _SERVER_LOCK
#define _SERVER_LOCK

#include <mutex>

//Clase Lock que maneja con RAII
//los mutex que se usan en el IndexHandler
class Lock {
	private:
		std::mutex &m;

	public:
		explicit Lock(std::mutex &m);
		
		~Lock();

		Lock(const Lock&) = delete;
		Lock& operator=(const Lock&) = delete;
		Lock(Lock&&) = delete;
		Lock& operator=(Lock&&) = delete;
};

#endif
