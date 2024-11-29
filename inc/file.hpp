#ifndef FILE_HPP
# define FILE_HPP

# include "unique_fd.hpp"
# include <stdexcept>
# include <fcntl.h>

class file : public unique_fd {
	public:
		file() noexcept = default;

		template <typename... Args>
		file(const char *path, Args... args) 
		: unique_fd(::open(path, args...)) {
			if (*this == -1) {
				throw std::runtime_error("Failed to open file");
			}
		}

		~file() noexcept = default;

		file(const file &) = delete;

		file(file &&) noexcept = default;

		file &operator=(const file &) = delete;

		file &operator=(file &&) noexcept = default;

		operator int() const noexcept;

		void lock() const;

		void unlock() const noexcept;
};

class guard_lock {
	private:
		const file &_file;
	public:
		guard_lock(const file &file) : _file(file) {
			_file.lock();
		}

		~guard_lock() noexcept {
			_file.unlock();
		}

		guard_lock() = delete;

		guard_lock(const guard_lock &) = delete;

		guard_lock(guard_lock &&) = delete;

		guard_lock &operator=(const guard_lock &) = delete;

		guard_lock &operator=(guard_lock &&) = delete;
};

#endif // FILE_HPP
