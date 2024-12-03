#include "client_manager.hpp"

client_manager &client_manager::_shared() {
	static client_manager instance;
	return instance;
}

client &client_manager::add(net::socket &&socket) {
	auto &clients = _shared()._clients;
	auto pair = clients.emplace(socket, std::move(socket));
	return pair.first->second;
}

void client_manager::del(const int fd) {
	auto &clients = _shared()._clients;
	clients.erase(fd);
}

auto client_manager::size() noexcept -> size_t {
	return _shared()._clients.size();
}
