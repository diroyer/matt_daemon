#include "server.hpp"
#include "tintin_reporter.hpp"
#include "running.hpp"
#include <iostream>
#include "client_manager.hpp"
#include <unistd.h>

server::server()
: _socket{AF_INET, SOCK_STREAM} {

	_socket.reuse_address();
	_socket.bind(net::addr{in_addr{INADDR_ANY}, 4242});
	_socket.listen();

	_socket.non_blocking();

	epoll::add(*this);

	Tintin_reporter::server("Server started");	
}

int server::fd() const noexcept {
	return _socket;
}

void server::read() {

	net::socket client_sock = _socket.accept();

	if (client_manager::size() >= 3) {
		Tintin_reporter::server("too many clients");
		client_sock.shutdown();
		return;
	}

	client_sock.non_blocking();

	auto &client = client_manager::add(std::move(client_sock));

	epoll::add(client);

	Tintin_reporter::server("new client connected");
}

void server::write() {
}

void server::disconnect() {
}

server::~server() noexcept {
	epoll::del(*this);
	Tintin_reporter::server("server stopped");
}

void server::run() {

	while (running::state() == true) {

		epoll::poll();
		//inotify::watch();
	}
}
