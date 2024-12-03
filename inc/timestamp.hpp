#ifndef TIMESTAMP_HPP
# define TIMESTAMP_HPP

# include <string_view>


class timestamp final {

	private:

		char _buffer[22U];

	public:

		using time_array = char[22U];

		timestamp(void) noexcept;
		timestamp(const timestamp&) noexcept = default;
		timestamp(timestamp&&) noexcept = default;
		timestamp& operator=(const timestamp&) noexcept = default;
		timestamp& operator=(timestamp&&) noexcept = default;
		~timestamp(void) noexcept = default;

		const time_array& time(void) const noexcept;

		unsigned size(void) const noexcept;


		auto view(void) const noexcept -> std::string_view;
};



#endif // TIMESTAMP_HPP
