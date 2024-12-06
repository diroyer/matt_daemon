#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

# include <unistd.h>
# include <sys/stat.h>

# include "timestamp.hpp"
# include "file.hpp"


class Tintin_reporter final {


	private:

		template <size_t N>
		static void _report(const char (&type)[N], const char *message) {

			constexpr const char* folder[] {
				"/var",
				"/var/log",
				"/var/log/matt_daemon",
				nullptr
			};

			for (size_t i = 0U; folder[i] != nullptr ; ++i) {

				struct ::stat st;

				if (::stat(folder[i], &st) == -1) {

					if (errno != ENOENT)
						return;

					if (::mkdir(folder[i], 0755) == -1)
						return;
				}

				if (!S_ISDIR(st.st_mode))
					return;
			}


			timestamp ts;

			const int fd = ::open("/var/log/matt_daemon/matt_daemon.log",
								O_WRONLY | O_APPEND | O_CREAT);

			if (fd == -1)
				return;

			static_cast<void>(::write(fd, ts.time(), ts.size()));
			static_cast<void>(::write(fd, type, N));
			static_cast<void>(::write(fd, message, __builtin_strlen(message)));
			static_cast<void>(::write(fd, "\n", 1U));

			static_cast<void>(::close(fd));
		}




	public:

		Tintin_reporter(void) = delete;
		Tintin_reporter(const Tintin_reporter&) = delete;
		Tintin_reporter(Tintin_reporter&&) = delete;
		Tintin_reporter &operator=(const Tintin_reporter&) = delete;
		Tintin_reporter &operator=(Tintin_reporter&&) = delete;
		~Tintin_reporter(void) = delete;


	public:

		static void info(const char* message) {
			_report("\x1b[32m    info\x1b[0m -> ", message);
		}

		static void error(const char* message) {
			_report("\x1b[31m   error\x1b[0m -> ", message);
		}

		static void signal(const char* message) {
			_report("\x1b[33m  signal\x1b[0m -> ", message);
		}

		static void server(const char* message) {
			_report("\x1b[34m  server\x1b[0m -> ", message);
		}

};

#endif // TINTIN_REPORTER_HPP
