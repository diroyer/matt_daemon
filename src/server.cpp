#include "server.hpp"
#include "tintin_reporter.hpp"
#include "running.hpp"
#include <iostream>
#include "client_manager.hpp"

server::server() : _socket{AF_INET, SOCK_STREAM} {
	//unlink_guard guard(sock_file);

	_socket.bind(net::addr{in_addr{INADDR_ANY}, 4242});
	_socket.listen();

	_socket.non_blocking();

	_epoll.add(*this);

	Tintin_reporter::report("Server started");	

	//guard.complete();
}

int server::fd() const noexcept {
	return _socket;
}

void server::read() {

	net::socket client_sock = _socket.accept();

	if (client_manager::size() >= 3) {
		Tintin_reporter::report("Too many clients");
		client_sock.shutdown();
		return;
	}

	client_sock.non_blocking();

	auto &client = client_manager::add(std::move(client_sock));

	_epoll.add(client);

	Tintin_reporter::report("New client connected");
}

void server::write() {
}

void server::disconnect() {
}

server::~server() noexcept {
	Tintin_reporter::report("Server stopped");
	//unlink(sock_file);
}

void server::run() {

	while (running::state() == true) {

		_epoll.wait();
	}
}
