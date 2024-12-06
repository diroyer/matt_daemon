#include "daemon.hpp"
#include "tintin_reporter.hpp"


int	main(void) {

	try {

		Deamon deamon;
		deamon.run();
		return EXIT_SUCCESS;
	}

	catch (const std::exception &e) {

		Tintin_reporter::error(e.what());
		return EXIT_FAILURE;
	}
}
