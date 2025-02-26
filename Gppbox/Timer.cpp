﻿#include "Timer.h"

Timer::Timer(const std::chrono::duration<double> duration)
	:
	duration(duration) {
	start();
}

bool Timer::isFinished() const {
	return std::chrono::steady_clock::now() >= endTime;
}

void Timer::start() {
	startTime = std::chrono::steady_clock::now();
	endTime = startTime + duration;
}

void Timer::restart() {
	startTime = endTime;
	endTime = startTime + duration;
}
