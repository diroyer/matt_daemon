#include "daemon.hpp"
#include "tintin_reporter.hpp"
#include "running.hpp"
#include <vector>

#include <sys/inotify.h>

#include "unique_fd.hpp"
#include "ioevent.hpp"



class inotify : public ioevent {

	private:

		unique_fd _fd;


	public:

		inotify(void)
		: _fd{::inotify_init()}  {

			if (_fd == -1)
				throw std::runtime_error("Failed to create inotify instance.");

		}

		~inotify(void) noexcept = default;



		auto add(const char* path, const ::uint32_t& mask) -> void {

			const int wd = ::inotify_add_watch(_fd, path, mask);

			if (wd == -1)
				throw std::runtime_error("Failed to add watch to inotify instance.");

			std::cout << "add wd: "<< wd << std::endl;
		}

		auto fd(void) const noexcept -> int override {
			return _fd;
		}

		auto read(void) -> void override {
			std::cout << "read" << std::endl;
			watch();
		}

		auto write(void) -> void override {
		}

		auto disconnect(void) -> void override {
		}


	private:

		auto watch(void) -> void {

			char buffer[4096] __attribute__((aligned(__alignof__(struct inotify_event))));

			const auto readed = ::read(_fd,
					buffer,
					sizeof(buffer));


			if (readed == -1)
				throw std::runtime_error("Failed to read inotify instance.");

			std::cout << "readed: " << readed << std::endl;

			std::cout << "START LOOP -----------------" << std::endl;

			for (size_t i = 0U; i < static_cast<size_t>(readed);) {

				const auto& event = *reinterpret_cast<const struct ::inotify_event*>(buffer + i);

				std::cout << "wd: "<< event.wd << std::endl;
				std::cout << "mask: "<< event.mask << std::endl;
				std::cout << "len: " << event.len << std::endl;
				std::cout << "name: " << std::flush;

				::write(STDOUT_FILENO, event.name, event.len);
				std::cout << std::endl;

				if (event.mask & IN_IGNORED) {
					std::cout << "File ignored" << std::endl;
				}

				std::cout << "delete mask: " << (event.mask & IN_DELETE) << std::endl;
				if (event.mask & IN_DELETE) {
					std::cout << "File deleted" << std::endl;
				}

				std::cout << "create mask: " << (event.mask & IN_CREATE) << std::endl;
				if (event.mask & IN_CREATE) {
					std::cout << "File created" << std::endl;
				}

				std::cout << "modify mask: " << (event.mask & IN_MODIFY) << std::endl;
				if (event.mask & IN_MODIFY) {
					std::cout << "File modified" << std::endl;
				}

				i += sizeof(struct ::inotify_event) + event.len;
			}

			std::cout << "END LOOP -----------------" << std::endl;
		}

};

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
