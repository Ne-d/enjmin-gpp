#pragma once

#include <chrono>

class Timer {
public:
	Timer() = default;
	explicit Timer(std::chrono::duration<double> duration);

	[[nodiscard]] bool isFinished();
	void start();
	void start(std::chrono::duration<double> duration);

	void skip();

	std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>> startTime;
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double>> endTime;
	
private:
	bool shouldSkip = false;

	std::chrono::duration<double> duration;
};
