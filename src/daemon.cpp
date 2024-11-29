#include "daemon.hpp"
#include <stdexcept>
#include <unistd.h>
#include <sys/stat.h>

void Deamon::_is_root() {
	if (::getuid() != 0) {
		throw std::runtime_error("You must be root to run this program.");
	}
}

pid_t Deamon::_fork() {
	const pid_t pid = ::fork();
	if (pid == -1) {
		throw std::runtime_error("Fork failed.");
	}
	return pid;
}

Deamon::Deamon() {
	_is_root();
}

void Deamon::_new_session() {
	if (::setsid() == -1) {
		throw std::runtime_error("Failed to create a new session.");
	}
}

void Deamon::_go_to_root() {
	if (::chdir("/") == -1) {
		throw std::runtime_error("Failed to change directory to root.");
	}
}

void Deamon::_close_fds() {
		::close(STDIN_FILENO);
		::close(STDOUT_FILENO);
		::close(STDERR_FILENO);
}

void Deamon::run() {
	/* Fork to creat orphan process */
	if (_fork() != 0) {
		return;
	}

	_new_session();

	/* Fork to disassociate from terminal */
	if (_fork() != 0) {
		return;
	}

	_go_to_root();
	umask(0);
	_close_fds();
}
