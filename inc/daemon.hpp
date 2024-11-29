#ifndef MATT_DEAMON_HPP
#define MATT_DEAMON_HPP

#include <sys/types.h>


class Deamon {
	public:
		Deamon();
		~Deamon() noexcept = default; 
		void run();
	private:
		static void _is_root();
		static void _new_session();
		static pid_t _fork();
		static void _go_to_root();
		static void _close_fds();
};

#endif // MATT_DEAMON_HPP
