#ifndef FD_HPP
# define FD_HPP

class unique_fd {
	protected:
		int _fd;
	public:
		unique_fd() noexcept;

		unique_fd(int) noexcept;

		~unique_fd() noexcept;

		unique_fd(const unique_fd &) = delete;

		unique_fd(unique_fd &&) noexcept;

		unique_fd &operator=(const unique_fd &) = delete;

		unique_fd &operator=(unique_fd &&) noexcept;

		operator int() const noexcept;
};

#endif // FD_HPP
