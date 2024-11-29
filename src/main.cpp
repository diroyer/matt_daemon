#include "daemon.hpp"
#include <stdexcept>
#include <iostream>
#include "file.hpp"

int	main(void) {
	try {
		file fl("/var/lock/daemon.lock", O_CREAT | O_RDONLY);
		guard_lock guard(fl);
		Deamon deamon;
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

}
