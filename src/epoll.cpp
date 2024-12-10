#include "epoll.hpp"
#include "tintin_reporter.hpp"

// #include "inotify.hpp"



auto epoll::_shared(void) -> epoll& {
	static epoll instance;
	return instance;
}

epoll::epoll()
: _epoll{::epoll_create1(0)} {

	if (_epoll == -1)
		throw std::runtime_error("epoll_create1");

	//add(inotify::instance());
}

void epoll::add(ioevent& io) {

	struct ::epoll_event ev {
		.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP,
		.data = { .ptr = &io }
	};

	if (::epoll_ctl(_shared()._epoll, EPOLL_CTL_ADD, io.fd(), &ev) == -1)
		throw std::runtime_error("epoll_ctl");
}

void epoll::del(const ioevent& io) noexcept {

	static_cast<void>(::epoll_ctl(_shared()._epoll,
								  EPOLL_CTL_DEL,
								  io.fd(),
								  nullptr));
}

ioevent& epoll::_data(epoll_event& event) noexcept {
	return *static_cast<ioevent*>(event.data.ptr);
}

void epoll::poll(void) {

	struct ::epoll_event buffer[3U + 1U];

	// wait for events
	const int result = ::epoll_wait(_shared()._epoll, buffer, 4U, -1);

	// TODO: handle signals
	if (result < 0) {
		if (errno == EINTR) {
			return;
		}
		throw std::runtime_error("epoll_wait");
	}

	// loop over events
	for (int i = 0; i < result; ++i) {

		ioevent& io = _data(buffer[i]);
		const auto events = buffer[i].events;

		if (events & EPOLLERR 
		 || events & EPOLLHUP 
		 || events & EPOLLRDHUP) {
			io.disconnect();

		} else if (events & EPOLLIN) {
			io.read();
		}
	}
}
