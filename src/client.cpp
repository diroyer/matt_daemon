#include "client.hpp"
#include "tintin_reporter.hpp"
#include "running.hpp"
#include "client_manager.hpp"

int client::fd() const noexcept {
	return _socket;
}

void client::read() {
	Tintin_reporter::report("client::read()");

	char buf[1024];
	std::string msg;

	while (true) {
		const ssize_t n = ::recv(_socket, buf, sizeof(buf), 0);

		if (n == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break;
			}
			throw std::runtime_error("recv() failed");
		}

		if (n == 0) {
			disconnect();
			break;
		}

		msg.append(buf, static_cast<size_t>(n));
	}

	if (msg.empty()) {
		return;
	}

	if (msg == "quit") {
		Tintin_reporter::report("Client sent quit command");
		running::stop();
		return;
	}

	Tintin_reporter::report(msg.data());
}

void client::write() {

}

void client::disconnect() {
	Tintin_reporter::report("client::disconnect()");
	client_manager::del(_socket);

}
