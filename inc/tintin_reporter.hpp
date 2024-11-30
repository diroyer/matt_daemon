#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP

class Tintin_reporter {
	public:
		Tintin_reporter() = delete;
		Tintin_reporter(const Tintin_reporter &src) = delete;
		Tintin_reporter &operator=(const Tintin_reporter &src) = delete;
		~Tintin_reporter() = delete;

		static void report(const char *message);
		static bool check_folder(void);
};

#endif // TINTIN_REPORTER_HPP
