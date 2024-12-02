#include "tintin_reporter.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "file.hpp"
#include "sys/stat.h"

#define log_folder "/var/log/matt_daemon"
#define log_file "/var/log/matt_daemon/matt_daemon.log"

bool Tintin_reporter::check_folder(void) {
	static const char *folder[] = 
	{"/var", "/var/log", "/var/log/matt_daemon", nullptr};

	for (size_t i = 0; folder[i] != nullptr ; ++i) {
		if (::access(folder[i], F_OK) == -1) {
			if (::mkdir(folder[i], 0755) == -1) {
				return false;
			}
		}
	}
	return true;
}

void Tintin_reporter::report(const char *message) {

	if (check_folder() == false) {
		return;
	}

	unique_fd fd(::open(log_file, O_CREAT | O_WRONLY | O_APPEND, 0644));
	if (fd == -1) {
		return;
	}

	static_cast<void>(::write(fd, message, strlen(message)));
	static_cast<void>(::write(fd, "\n", 1));
}
