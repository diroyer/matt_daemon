#ifndef EPOLL_HPP
# define EPOLL_HPP

#include "ioevent.hpp"
#include "unique_fd.hpp"

#include <sys/epoll.h>
#include <stdexcept>


class epoll final {

	private:

		unique_fd _epoll;

	public:

		epoll();

		epoll(const epoll&) = delete;
		epoll(epoll&&) noexcept = default;
		epoll& operator=(const epoll&) = delete;
		epoll& operator=(epoll&&) noexcept = default;

		~epoll() noexcept = default;


		void add(ioevent&) const;


		void del(const ioevent& io) const noexcept;


		void wait() const;

		ioevent& data(epoll_event&) const noexcept;
};

#endif // EPOLL_HPP
