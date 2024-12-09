#include "daemon.hpp"
#include "tintin_reporter.hpp"


int	main(void) {

	try {
		daemon::run();
		return EXIT_SUCCESS;
	}

	catch (const std::exception &e) {

		Tintin_reporter::error(e.what());
		return EXIT_FAILURE;
	}
}
