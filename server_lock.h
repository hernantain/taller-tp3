#ifndef _SERVER_LOCK
#define _SERVER_LOCK

#include <mutex>

class Lock {
	private:
		std::mutex &m;

	public:
		Lock(std::mutex &m);
		
		~Lock();

		Lock(const Lock&) = delete;
		Lock& operator=(const Lock&) = delete;
		Lock(Lock&&) = delete;
		Lock& operator=(Lock&&) = delete;
};

#endif
