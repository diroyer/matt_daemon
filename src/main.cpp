#include "daemon.hpp"
#include <stdexcept>
#include <iostream>
#include "file.hpp"
#include <unistd.h>
#include <cstring>
#include "tintin_reporter.hpp"
#include "socket.hpp"
#include "server.hpp"
#include "running.hpp"

#include <signal.h>

int	main(void) {
	try {

		Deamon deamon;
		deamon.run();
	
		return 0;
	} catch (const std::exception &e) {
		Tintin_reporter::report(e.what());
		return 1;
	}
}
