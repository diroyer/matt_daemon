#ifndef INOTIFY_HPP
# define INOTIFY_HPP


#include "ioevent.hpp"
#include "unique_fd.hpp"
#include "epoll.hpp"

#include <unistd.h>
#include <sys/inotify.h>
#include <iostream>
#include <stdexcept>



class inotify : public ioevent {


	private:

		// -- private members -------------------------------------------------

		/* inotify descriptor */
		unique_fd _fd;


		// -- private static methods ------------------------------------------

		static auto _shared(void) -> inotify& {
			static inotify instance;
			return instance;
		}


		// -- private lifecycle -----------------------------------------------

		inotify(void)
		: _fd{::inotify_init()}  {

			// check error
			if (_fd == -1)
				throw std::runtime_error("Failed to create inotify instance.");

			// add to epoll
			epoll::add(*this);
		}

		~inotify(void) noexcept {

			// remove from epoll
			epoll::del(*this);
		}


	public:

		// -- public static methods -------------------------------------------

		static auto add(const char* path, const ::uint32_t& mask) -> void {

			const int wd = ::inotify_add_watch(_shared()._fd, path, mask);

			if (wd == -1)
				throw std::runtime_error("Failed to add watch to inotify instance.");
		}


		// -- public overrides ------------------------------------------------

		auto fd(void) const noexcept -> int override {
			return _fd;
		}

		auto read(void) -> void override {
			_handle_read();
		}

		auto write(void) -> void override {
		}

		auto disconnect(void) -> void override {
		}


	private:

		// -- private methods -------------------------------------------------

		auto _handle_read(void) -> void {

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

#endif // INOTIFY_HPP
