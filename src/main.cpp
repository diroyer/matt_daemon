#include "daemon.hpp"
#include <stdexcept>
#include <iostream>
#include "file.hpp"
#include <unistd.h>
#include <cstring>
#include "tintin_reporter.hpp"
#include "socket.hpp"
#include "server.hpp"

int	main(void) {
	try {

		server server;
		return 0;

		Deamon deamon;
		deamon.run();
		Tintin_reporter::report("Daemon started\n");
		return 0;
	} catch (const std::exception &e) {
		Tintin_reporter::report(e.what());
		return 1;
	}
}
