#include "signal.hpp"
#include "running.hpp"
#include "tintin_reporter.hpp"
#include <csignal>

static void handler(int sig) {
	switch (sig) {
		case SIGTSTP:
			Tintin_reporter::signal("SIGTSTP received");
			break;
		case SIGCONT:
			Tintin_reporter::signal("SIGCONT received");
			break;
		case SIGINT:
			Tintin_reporter::signal("SIGINT received");
			running::stop();
			break;
		case SIGTERM:
			Tintin_reporter::signal("SIGTERM received");
			running::stop();
			break;
		case SIGQUIT:
			Tintin_reporter::signal("SIGQUIT received");
			running::stop();
			break;
		case SIGABRT:
			Tintin_reporter::signal("SIGABRT received");
			running::stop();
			break;
		case SIGHUP:
			Tintin_reporter::signal("SIGHUP received");
			running::stop();
			break;
		case SIGSEGV:
			Tintin_reporter::signal("SIGSEGV received");
			running::stop();
			break;
		default:
			Tintin_reporter::signal("unhandled signal received");
			running::stop();
			break;
	}
}

static void record(int sig) {
	if (::signal(sig, handler) == SIG_ERR) {
		throw std::runtime_error("Failed to record signal");
	}
}

template <int ...Args>
void record(void) {
	(record(Args), ...);
}

void sig::init() {
	record<SIGTSTP, SIGCONT, SIGINT, SIGTERM, SIGQUIT, SIGABRT, SIGHUP, SIGSEGV>();
}
