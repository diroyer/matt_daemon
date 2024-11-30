#include "socket.hpp"
#include <stdexcept>

socket::socket(int domain, int type, int protocol) 
: unique_fd(::socket(domain, type, protocol)) {
	if (*this == -1) {
		throw std::runtime_error("Failed to create socket");
	}
}

socket::operator int() const noexcept {
	return unique_fd::operator int();
}

addr::addr() noexcept : _addr{}, _len{0} {}

addr::addr(in_addr ip, in_port_t port)
	: _addr{}, _len{sizeof(sockaddr_in)} {
		auto &addr = as_ipv4();
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = ip.s_addr;

}

addr::addr(in6_addr ip, in_port_t port)
	: _addr{}, _len{sizeof(sockaddr_in6)} {
		auto &addr = as_ipv6();
		addr.sin6_family = AF_INET6;
		addr.sin6_port = htons(port);
		addr.sin6_addr = ip;
}

addr::addr(const std::string &path)
	: _addr{}, _len{sizeof(sockaddr_un)} {
		auto &addr = as_local();
		addr.sun_family = AF_UNIX;

		for (size_t i = 0; i < sizeof(addr.sun_path) - 1; ++i) {
			addr.sun_path[i] = path[i];
			if (path[i] == '\0') {
				break;
			}
		}
}

socklen_t &addr::size() noexcept {
	return _len;
}

const socklen_t &addr::size() const noexcept {
	return _len;
}

const sockaddr_in &addr::as_ipv4() const noexcept {
	return *reinterpret_cast<const sockaddr_in *>(&_addr);
}

const sockaddr_in6 &addr::as_ipv6() const noexcept {
	return *reinterpret_cast<const sockaddr_in6 *>(&_addr);
}

const sockaddr &addr::as_sockaddr() const noexcept {
	return *reinterpret_cast<const sockaddr *>(&_addr);
}

const sockaddr_un &addr::as_local() const noexcept {
	return *reinterpret_cast<const sockaddr_un *>(&_addr);
}

sockaddr_in &addr::as_ipv4() noexcept {
	return *reinterpret_cast<sockaddr_in *>(&_addr);
}

sockaddr_in6 &addr::as_ipv6() noexcept {
	return *reinterpret_cast<sockaddr_in6 *>(&_addr);
}

sockaddr &addr::as_sockaddr() noexcept {
	return *reinterpret_cast<sockaddr *>(&_addr);
}

sockaddr_un &addr::as_local() noexcept {
	return *reinterpret_cast<sockaddr_un *>(&_addr);
}

void socket::bind(const addr &addr) const {
	if (::bind(*this, &addr.as_sockaddr(), addr.size()) == -1) {
		throw std::runtime_error("Failed to bind socket");
	}
}

void socket::listen(int backlog) const {
	if (::listen(*this, backlog) == -1) {
		throw std::runtime_error("Failed to listen on socket");
	}
}

class socket socket::accept(addr &addr) const {
	class socket client;
	
	const int fd = ::accept(*this, &addr.as_sockaddr(), &addr.size());
	if (fd == -1) {
		throw std::runtime_error("Failed to accept connection");
	}
	client._fd = fd;
	return client;
}

class socket socket::accept() const {
	class socket client;
	
	const int fd = ::accept(*this, nullptr, nullptr);
	if (fd == -1) {
		throw std::runtime_error("Failed to accept connection");
	}
	client._fd = fd;
	return client;
}
