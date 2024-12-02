#include "server.hpp"
#include "tintin_reporter.hpp"
#include "running.hpp"
#include <iostream>

server::server() : _socket{AF_UNIX, SOCK_STREAM} {
	unlink_guard guard(sock_file);

	_socket.bind(net::addr{sock_file});
	_socket.listen();

	// add server to epoll
	_epoll.add(*this);

	Tintin_reporter::report("Server started");	

	guard.complete();
}

int server::fd() const noexcept {
	return _socket;
}

void server::read() {
	net::socket client_sock = _socket.accept();
	_clients.emplace(client_sock, std::move(client_sock));

	Tintin_reporter::report("New client connected");

}

void server::write() {
}

void server::disconnect() {
}

server::~server() noexcept {
	Tintin_reporter::report("Server stopped");
	unlink(sock_file);
}



void server::run() {

	while (running::state() == true) {

		_epoll.wait();
		Tintin_reporter::report(".");
	}
}
