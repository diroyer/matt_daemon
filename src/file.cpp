#include "file.hpp"
#include <sys/file.h>

file::operator int() const noexcept {
	return unique_fd::operator int();
}

void file::lock() const {
	if (::flock(*this, LOCK_EX | LOCK_NB) == -1) {
		throw std::runtime_error("Failed to lock file");
	}
}

void file::unlock() const noexcept {
	static_cast<void>(::flock(*this, LOCK_UN));
}
