#include "running.hpp"


running& running::_shared() noexcept {
	static running instance;
	return instance;
}

running::running() noexcept
: _state{true} {
}

auto running::state() noexcept -> bool {
	return _shared()._state;
}

auto running::stop() noexcept -> void {
	_shared()._state = false;
}

auto running::start() noexcept -> void {
	_shared()._state = true;
}
