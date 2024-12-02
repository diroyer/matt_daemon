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

// signal handler
void signal_handler(int signum) {

	switch (signum) {
		case SIGINT:
			//Tintin_reporter::report("SIGINT received");
			Tintin_reporter::print("SIGINT received", signum);
			running::stop();
			break;
		case SIGTERM:
			//Tintin_reporter::report("SIGTERM received");
			Tintin_reporter::print("SIGTERM received", signum);
			running::stop();
			break;
		default:
			break;
	}
}

int	main(void) {
	try {

		::signal(SIGINT, signal_handler);
		::signal(SIGTERM, signal_handler);


		server server;
		server.run();
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
