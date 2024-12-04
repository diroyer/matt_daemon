#ifndef IOEVENT_HPP
# define IOEVENT_HPP


class ioevent {


	public:

		virtual int fd(void) const noexcept = 0;
		virtual void read(void) = 0;
		virtual void write(void) = 0;
		virtual void disconnect(void) = 0;

		ioevent(void) noexcept = default;
		ioevent(const ioevent&) noexcept = default;
		ioevent(ioevent&&) noexcept = default;
		virtual ~ioevent(void) noexcept = default;

		auto operator=(const ioevent&) noexcept -> ioevent& = default;
		auto operator=(ioevent&&) noexcept -> ioevent& = default;
};

#endif // IOVENT_HPP
