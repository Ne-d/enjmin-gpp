#include "Timer.h"

Timer::Timer(const std::chrono::duration<double> duration)
	:
	duration(duration) {
	start();
}

bool Timer::isFinished() {
	if (shouldSkip) {
		shouldSkip = false;
		return true;
	}
	
	return std::chrono::steady_clock::now() >= endTime;
}

void Timer::start() {
	startTime = std::chrono::steady_clock::now();
	endTime = startTime + duration;
}

void Timer::start(const std::chrono::duration<double> duration) {
	this->duration = duration;
	start();
}

void Timer::skip() {
	shouldSkip = true;
}
