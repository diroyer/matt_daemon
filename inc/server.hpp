#ifndef SERVER_HPP
# define SERVER_HPP

#include "ioevent.hpp"
#include "socket.hpp"
#include <unordered_map>
#include <unistd.h>
#include "client.hpp"
#include "epoll.hpp"

#define sock_file "/tmp/matt_daemon.sock"


class unlink_guard final {
	private:
		std::string _path;
		bool _completed;

	public:
		unlink_guard() = delete;
		unlink_guard(std::string &&path) noexcept
		: _path{std::move(path)}, _completed{false} {}
		~unlink_guard() noexcept {
			if (not _completed) {
				static_cast<void>(unlink(_path.data()));
			}
		}

		void complete() noexcept {
			_completed = true;
		}

		unlink_guard(const unlink_guard &) = delete;
		unlink_guard(unlink_guard &&) = delete;
		unlink_guard &operator=(const unlink_guard &) = delete;
		unlink_guard &operator=(unlink_guard &&) = delete;

};

class server : public ioevent {
	private:
		net::socket _socket;
		std::unordered_map<int, client> _clients;
		epoll _epoll;

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
