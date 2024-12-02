#ifndef RUNNING_HPP
#define RUNNING_HPP


class running final {

	private:

		bool _state;

		static running& _shared() noexcept;

		running() noexcept;

		running(const running&) = delete;
		running(running&&) = delete;
		auto operator=(const running&) -> running& = delete;
		auto operator=(running&&) -> running& = delete;


	public:

		static auto state() noexcept -> bool;

		static auto stop() noexcept -> void;

		static auto start() noexcept -> void;
};

#endif // RUNNING_HPP
