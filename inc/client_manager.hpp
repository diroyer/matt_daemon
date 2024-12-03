#ifndef CLIENT_MANAGER_HPP
# define CLIENT_MANAGER_HPP

#include "client.hpp"
#include <unordered_map>

class client_manager final {
	private:
		std::unordered_map<int, client> _clients;
		static client_manager &_shared();

		client_manager() = default;
		~client_manager() noexcept = default;

		client_manager(const client_manager &) = delete;
		client_manager(client_manager &&) = delete;
		auto operator=(const client_manager &) -> client_manager & = delete;
		auto operator=(client_manager &&) -> client_manager & = delete;

	public:

		static client& add(net::socket &&socket);
		static void del(const int fd);
		static auto size() noexcept -> size_t;
};

#endif // CLIENT_MANAGER_HPP
