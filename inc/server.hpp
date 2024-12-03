#ifndef SERVER_HPP
# define SERVER_HPP

#include "ioevent.hpp"
#include "socket.hpp"
#include "epoll.hpp"

class server : public ioevent {
	private:
		net::socket _socket;

	public:
		server();
		server(const server &) = delete;
		server(server &&) noexcept = default;
		server &operator=(const server &) = delete;
		server &operator=(server &&) noexcept = default;
		~server() noexcept;

		int fd() const noexcept override;
		void read() override;
		void write() override;
		void disconnect() override;

		void run();
};

#endif // SERVER_HPP
