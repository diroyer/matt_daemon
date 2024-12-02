#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ioevent.hpp"
#include "socket.hpp"

class client : public ioevent {
	private:
		net::socket _socket;

	public:
		client() noexcept = default;
		client(net::socket &&socket) noexcept : _socket{std::move(socket)} {}


		client(const client &) = delete;
		client(client &&) noexcept = default;
		client &operator=(const client &) = delete;
		client &operator=(client &&) noexcept = default;
		~client() noexcept = default;

		int fd() const noexcept override;
		void read() override;
		void write() override;
		void disconnect() override;
};

#endif // CLIENT_HPP
