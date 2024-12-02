#include "epoll.hpp"


epoll::epoll()
: _epoll{::epoll_create1(0)} {

	if (_epoll == -1)
		throw std::runtime_error("epoll_create1");
}

void epoll::add(ioevent& io) const {

	struct ::epoll_event ev {
		.events = EPOLLIN | EPOLLERR | EPOLLHUP,
		.data = { .ptr = &io }
	};

	if (::epoll_ctl(_epoll, EPOLL_CTL_ADD, io.fd(), &ev) == -1)
		throw std::runtime_error("epoll_ctl");
}

void epoll::del(const ioevent& io) const noexcept {

	static_cast<void>(::epoll_ctl(_epoll, EPOLL_CTL_DEL, io.fd(), nullptr));
}

void epoll::wait() const {

	// 20ms timeout
	constexpr int timeout = 200;

	struct ::epoll_event buffer[3U + 1U];

	// wait for events
	const int result = ::epoll_wait(_epoll, buffer, 4U, timeout);

	// TODO: handle signals
	if (result < 0) {
		if (errno == EINTR)
			return;
		throw std::runtime_error("epoll_wait");
	}

	// loop over events
	for (int i = 0; i < result; ++i) {

		ioevent& io = *static_cast<ioevent*>(buffer[i].data.ptr);

		if (buffer[i].events & EPOLLERR
		 || buffer[i].events & EPOLLHUP)
			io.disconnect();

		if (buffer[i].events & EPOLLIN)
			io.read();
	}
}
