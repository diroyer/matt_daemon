#include "socket.hpp"
#include <stdexcept>
#include <unistd.h>

net::socket::socket(int domain, int type, int protocol) 
: unique_fd(::socket(domain, type, protocol)) {
	if (*this == -1) {
		throw std::runtime_error("Failed to create socket");
	}
}

net::socket::operator int() const noexcept {
	return unique_fd::operator int();
}

void net::socket::bind(const net::addr &addr) const {
	if (::bind(*this, &addr.as_sockaddr(), addr.size()) == -1) {
		throw std::runtime_error("Failed to bind socket");
	}
}

void net::socket::listen(int backlog) const {
	if (::listen(*this, backlog) == -1) {
		throw std::runtime_error("Failed to listen on socket");
	}
}

net::socket net::socket::accept(addr &addr) const {
	class socket client;
	
	const int fd = ::accept(*this, &addr.as_sockaddr(), &addr.size());
	if (fd == -1) {
		throw std::runtime_error("Failed to accept connection");
	}
	client._fd = fd;
	return client;
}

net::socket net::socket::accept() const {
	class socket client;
	
	const int fd = ::accept(*this, nullptr, nullptr);
	if (fd == -1) {
		throw std::runtime_error("Failed to accept connection");
	}
	client._fd = fd;
	return client;
}

net::addr::addr() noexcept : _addr{}, _len{sizeof(sockaddr_storage)} {}

net::addr::addr(in_addr ip, in_port_t port)
	: _addr{}, _len{sizeof(sockaddr_in)} {
		auto &addr = as_ipv4();
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = ip.s_addr;

}

net::addr::addr(in6_addr ip, in_port_t port)
	: _addr{}, _len{sizeof(sockaddr_in6)} {
		auto &addr = as_ipv6();
		addr.sin6_family = AF_INET6;
		addr.sin6_port = htons(port);
		addr.sin6_addr = ip;
}

net::addr::addr(const std::string &path)
	: _addr{}, _len{sizeof(sockaddr_un)} {

		static_cast<void>(unlink(path.data()));
		auto &addr = as_local();
		addr.sun_family = AF_UNIX;

		for (size_t i = 0; i < sizeof(addr.sun_path) - 1; ++i) {
			addr.sun_path[i] = path[i];
			if (path[i] == '\0') {
				break;
			}
		}
}

socklen_t &net::addr::size() noexcept {
	return _len;
}

const socklen_t &net::addr::size() const noexcept {
	return _len;
}

const sockaddr_in &net::addr::as_ipv4() const noexcept {
	return *reinterpret_cast<const sockaddr_in *>(&_addr);
}

const sockaddr_in6 &net::addr::as_ipv6() const noexcept {
	return *reinterpret_cast<const sockaddr_in6 *>(&_addr);
}

const sockaddr &net::addr::as_sockaddr() const noexcept {
	return *reinterpret_cast<const sockaddr *>(&_addr);
}

const sockaddr_un &net::addr::as_local() const noexcept {
	return *reinterpret_cast<const sockaddr_un *>(&_addr);
}

sockaddr_in &net::addr::as_ipv4() noexcept {
	return *reinterpret_cast<sockaddr_in *>(&_addr);
}

sockaddr_in6 &net::addr::as_ipv6() noexcept {
	return *reinterpret_cast<sockaddr_in6 *>(&_addr);
}

sockaddr &net::addr::as_sockaddr() noexcept {
	return *reinterpret_cast<sockaddr *>(&_addr);
}

sockaddr_un &net::addr::as_local() noexcept {
	return *reinterpret_cast<sockaddr_un *>(&_addr);
}

