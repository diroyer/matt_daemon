#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include "timestamp.hpp"

#include "system/write.hpp"


class file_event {

	public:

		file_event(void) noexcept = default;
		virtual ~file_event(void) noexcept = default;

		virtual auto in_delete(void) -> void = 0;
		virtual auto in_create(void) -> void = 0;
		virtual auto in_modify(void) -> void = 0;
		virtual auto in_ignored(void) -> void = 0;
};

#include <sys/stat.h>

namespace sys {


	template <unsigned N>
	auto mkdir(const char (&path)[N], const mode_t& mode) -> void {
		if (::mkdir(path, mode) == -1)
			throw std::runtime_error("mkdir");
	}

	inline auto mkdir(const std::string_view& path, const mode_t& mode) -> void {
		if (::mkdir(path.data(), mode) == -1)
			throw std::runtime_error("mkdir");
	}

	template <unsigned N>
	auto stat(const char (&path)[N]) -> struct ::stat {
		struct ::stat st;

		if (::stat(path, &st) == -1)
			throw std::runtime_error("stat");

		return st;
	}

	inline auto stat(const std::string_view& path) -> struct ::stat {
		struct ::stat st;

		if (::stat(path.data(), &st) == -1)
			throw std::runtime_error("stat");

		return st;
	}

}

#include "file.hpp"


class Tintin_reporter : public file_event {


	private:

		// -- private members -------------------------------------------------

		/* log file */
		file _log;

		/* buffer */
		std::string _buffer;


		// -- private static members ------------------------------------------

		/* log file path */
		static constexpr const char* log_file = "/var/log/matt_daemon/matt_daemon.log";




		static void _setup(void);


		// -- private static methods ------------------------------------------

		/* get shared instance */
		static auto _shared(void) -> Tintin_reporter& {
			static Tintin_reporter instance;
			return instance;
		}


		// -- private lifecycle -----------------------------------------------

		/* default constructor */
		Tintin_reporter(void)
		: _log{log_file, O_CREAT | O_WRONLY | O_APPEND, 0644},
		  _buffer{} {

			_setup();

			// create folder and log file
			// inotify::add("/var/log/matt_daemon", IN_IGNORED, IN_DELETE);
		}


		Tintin_reporter(const Tintin_reporter&) = delete;
		Tintin_reporter &operator=(const Tintin_reporter&) = delete;

		~Tintin_reporter(void) noexcept = default;


	public:

		void in_delete(void) override {

			// recreate log file
			_log = file{log_file, O_CREAT | O_WRONLY | O_APPEND, 0644};
			// inotify::add("/var/log/matt_daemon", IN_IGNORED, IN_DELETE);
		}

		void in_create(void) override {
		}

		void in_modify(void) override {
		}

		void in_ignored(void) override {
			in_delete();
		}



	private:

		template <size_t N>
		void _report(const char (&type)[N], const char *message) {


			try {

				timestamp ts;

				_buffer.append(ts.view());
				_buffer.append(type);
				_buffer.append(message);
				_buffer.append("\n");

				sys::write(_log, _buffer);

				_buffer.clear();
			}

			catch (const std::exception& e) {
				// keep buffer
			}
		}


	public:

		static void info(const char* message) {
			_shared()._report("\x1b[32m    info\x1b[0m -> ", message);
		}

		static void error(const char* message) {
			_shared()._report("\x1b[31m   error\x1b[0m -> ", message);
		}
		static void signal(const char* message) {
			_shared()._report("\x1b[33m  signal\x1b[0m -> ", message);
		}
		static void server(const char* message) {
			_shared()._report("\x1b[34m  server\x1b[0m -> ", message);
		}



		//template <typename T>
		//static void _print(std::ostream &os, const T& arg) {
		//	os << arg << ' ';
		//}
		//
		//
		//template <typename... Ts>
		//static void print(Ts&&... args) {
		//
		//}

};

#endif // TINTIN_REPORTER_HPP
