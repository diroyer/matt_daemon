#include "unique_fd.hpp"
#include <unistd.h>

unique_fd::unique_fd() noexcept : _fd(-1) {}

unique_fd::unique_fd(int fd) noexcept : _fd(fd) {}

unique_fd::~unique_fd() noexcept {
	if (_fd != -1) {
		::close(_fd);
	}
}

unique_fd::unique_fd(unique_fd &&other) noexcept : _fd(other._fd) {
	other._fd = -1;
}

unique_fd &unique_fd::operator=(unique_fd &&other) noexcept {
	if (this != &other) {
		if (_fd != -1) {
			::close(_fd);
		}
		_fd = other._fd;
		other._fd = -1;
	}
	return *this;
}

unique_fd::operator int() const noexcept {
	return _fd;
}
