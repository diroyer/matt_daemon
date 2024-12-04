#include "daemon.hpp"
#include "tintin_reporter.hpp"
#include "running.hpp"
#include <vector>

#include <sys/inotify.h>

#include "unique_fd.hpp"

#include "epoll.hpp"

int	main(void) {


	/*
	std::cout << "alignof(struct ::inotify_event): " << alignof(struct ::inotify_event) << std::endl;
	std::cout << "sizeof(struct ::inotify_event): " << sizeof(struct ::inotify_event) << std::endl;
	return 0;
	*/

	/*
	try {

		const char* path = "yolo";

		inotify in;

		in.add(path, IN_CREATE | IN_DELETE | IN_MODIFY);

		epoll::add(in);

		while (true) {

			epoll::poll();
		}
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
	*/






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
