#include "timestamp.hpp"
#include <ctime>


timestamp::timestamp(void) noexcept
: _buffer{
	'[',
	'D', 'D',
	'/',
	'M', 'M',
	'/',
	'Y', 'Y', 'Y', 'Y',
	'-',
	'H', 'H',
	':',
	'M', 'M',
	':',
	'S', 'S',
	']',
	'\0'
} {

	// get time
	const ::time_t now = ::time(nullptr);

	// get time info
	const ::tm* const time = ::localtime(&now);


	if (time == nullptr)
		return;

	int value;

	/* day [1 - 2] */
	value = time->tm_mday;
	_buffer[ 1] = static_cast<char>(value / 10) + '0';
	_buffer[ 2] = static_cast<char>(value % 10) + '0';

	/* month [4 - 5] */
	value = time->tm_mon + 1;
	_buffer[ 4] = static_cast<char>(value / 10) + '0';
	_buffer[ 5] = static_cast<char>(value % 10) + '0';

	/* year [7 - 10] */
	value = time->tm_year + 1900;
	_buffer[ 7] = static_cast<char> (value / 1000) + '0';
	_buffer[ 8] = static_cast<char>((value /  100) % 10) + '0';
	_buffer[ 9] = static_cast<char>((value /   10) % 10) + '0';
	_buffer[10] = static_cast<char> (value %   10) + '0';

	/* hour [12 - 13] */
	value = time->tm_hour;
	_buffer[12] = static_cast<char>(value / 10) + '0';
	_buffer[13] = static_cast<char>(value % 10) + '0';

	/* minute [15 - 16] */
	value = time->tm_min;
	_buffer[15] = static_cast<char>(value / 10) + '0';
	_buffer[16] = static_cast<char>(value % 10) + '0';

	/* second [18 - 19] */
	value = time->tm_sec;
	_buffer[18] = static_cast<char>(value / 10) + '0';
	_buffer[19] = static_cast<char>(value % 10) + '0';
}

const timestamp::time_array& timestamp::time(void) const noexcept {
	return _buffer;
}

auto timestamp::view(void) const noexcept -> std::string_view {
	return std::string_view(_buffer, sizeof(_buffer) - 1U);
}

unsigned timestamp::size(void) const noexcept {
	return sizeof(_buffer);
}
