#include "tintin_reporter.hpp"
#include <fcntl.h>
#include "file.hpp"
#include "sys/stat.h"


void Tintin_reporter::_setup(void) {

	const char* folder[] {
		"/var",
		"/var/log",
		"/var/log/matt_daemon",
		nullptr
	};

	for (size_t i = 0U; folder[i] != nullptr ; ++i) {

		struct ::stat st;

		if (::stat(folder[i], &st) == -1) {

			sys::mkdir(folder[i], 0755);
		}

		//if (!S_ISDIR(st.st_mode))
		//	return -1;
	}
}


