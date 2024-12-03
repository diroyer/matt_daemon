#include "client.hpp"
#include "tintin_reporter.hpp"
#include "running.hpp"
#include "client_manager.hpp"
#include "epoll.hpp"

int client::fd() const noexcept {
	return _socket;
}

void client::read() {

	char buf[1024U];
	std::string msg;

	while (true) {

		const ssize_t n = ::recv(_socket, buf, sizeof(buf), 0);

		if (n == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break;
			}
			throw std::runtime_error("recv");
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
		Tintin_reporter::server("client sent 'quit'");
		running::stop();
		return;
	}

	std::string str{"reading from client: "};
	str.append(msg);

	Tintin_reporter::server(str.data());
}

void client::write() {

}

void client::disconnect() {
	Tintin_reporter::server("client disconnected");
	client_manager::del(_socket);
	epoll::del(*this);
}
