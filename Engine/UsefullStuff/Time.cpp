#include"Time.h"

TimePoint Time::GetTimePoint() {
	return std::chrono::steady_clock::now();
}

float Time::GetTime() {
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - ProgramStartTime).count();
}

float Time::GetDuration(const TimePoint& p1, const TimePoint& p2) {
	return std::chrono::duration<float>(p2 - p1).count();
}