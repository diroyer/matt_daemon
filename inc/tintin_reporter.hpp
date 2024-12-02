#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

#include <iostream>
#include <fstream>


//class timestamp final {
//
//	using time_array = char[26U];
//
//	inline const time_array& get_time(void) {
//
//	static time_array time {
//		'[', '2', '0', '2', '1', '-', '0', '1', '-', '0', '1', ' ', '0', '0', ':', '0', '0', ':', '0', '0', ']', '\0'
//	};
//
//	return time;
//}
//}



class Tintin_reporter {


	private:

		static constexpr const char *log_folder = "/var/log/matt_daemon";
		static constexpr const char *log_file = "/var/log/matt_daemon/matt_daemon.log";


	public:
		Tintin_reporter() = delete;
		Tintin_reporter(const Tintin_reporter &src) = delete;
		Tintin_reporter &operator=(const Tintin_reporter &src) = delete;
		~Tintin_reporter() = delete;

		static void report(const char *message);
		static bool check_folder(void);



		template <typename T>
		static void _print(std::ostream &os, const T& arg) {
			os << arg << ' ';
		}


		template <typename... Ts>
		static void print(Ts&&... args) {

			if (check_folder() == false) {
				return;
			}

			// open log file
			std::ofstream log_file(Tintin_reporter::log_file, std::ios::app);

			// write to log file
			(_print(log_file, args), ...);

			// close log file
			log_file.close();
		}

};

#endif // TINTIN_REPORTER_HPP
