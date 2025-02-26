#pragma once

#include <chrono>

class Timer {
public:
	explicit Timer(std::chrono::duration<double> duration);

	[[nodiscard]] bool isFinished() const;
	void start();

private:
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>> startTime;
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>> endTime;

	std::chrono::duration<double> duration;
};
