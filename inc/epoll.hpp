#ifndef EPOLL_HPP
# define EPOLL_HPP

#include "ioevent.hpp"
#include "unique_fd.hpp"

#include <sys/epoll.h>
#include <stdexcept>


class epoll final {

	private:

		unique_fd _epoll;

		static auto _shared(void) -> epoll&;

		epoll();

		epoll(const epoll&) = delete;
		epoll(epoll&&) noexcept = default;
		epoll& operator=(const epoll&) = delete;
		epoll& operator=(epoll&&) noexcept = default;

		~epoll() noexcept = default;

		static ioevent& _data(epoll_event&) noexcept;

	public:

		static void add(ioevent&);


		static void del(const ioevent& io) noexcept;


		static void poll(void);

};

#endif // EPOLL_HPP
