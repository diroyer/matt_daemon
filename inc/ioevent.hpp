#ifndef IOEVENT_HPP
# define IOEVENT_HPP

class ioevent {
	public:
		virtual int fd() const noexcept = 0;
		virtual void read() = 0;
		virtual void write() = 0;
		virtual void disconnect() = 0;

		virtual ~ioevent() noexcept = default;
		ioevent() noexcept = default;
		ioevent(const ioevent&) noexcept = default;
		ioevent& operator=(const ioevent&) noexcept = default;
		ioevent(ioevent&&) noexcept = default;
		ioevent& operator=(ioevent&&) noexcept = default;
};

#endif // IOVENT_HPP
