#ifndef SYSTEM_WRITE_HPP
# define SYSTEM_WRITE_HPP

#include <unistd.h>
#include <string>
#include <string_view>
#include <stdexcept>


namespace sys {


	inline auto write(const int& fd, const char* buffer, const size_t& size) -> void {

		if (::write(fd, buffer, size) == -1)
			throw std::runtime_error("write");
	}

	template <unsigned N>
	auto write(const int& fd, const char (&buffer)[N]) -> void {
		sys::write(fd, buffer, N);
	}

	inline auto write(const int& fd, const std::string_view& view) -> void {
		sys::write(fd, view.data(), view.size());
	}

	inline auto write(const int& fd, const std::string& str) -> void {
		sys::write(fd, str.data(), str.size());
	}
}

#endif // SYSTEM_WRITE_HPP
