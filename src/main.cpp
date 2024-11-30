#include "daemon.hpp"
#include <stdexcept>
#include <iostream>
#include "file.hpp"
#include <unistd.h>
#include <cstring>
#include "tintin_reporter.hpp"
#include "socket.hpp"

int	main(void) {
	try {
		class socket s(AF_UNIX, SOCK_STREAM);
		s.bind(addr("./yolo"));
		s.listen();

		Deamon deamon;
		deamon.run();
		Tintin_reporter::report("Daemon started\n");
		return 0;
	} catch (const std::exception &e) {
		Tintin_reporter::report(e.what());
		return 1;
	}
}
