#pragma once
#include <chrono>

using namespace std::chrono;

class Timer
{
private:
	//In seconds as well
	double maxSeconds;
	std::chrono::steady_clock::time_point startTime;
public:
	Timer(double maxSeconds);
	bool isRunning() const;
	//In seconds
	double getRemainingTime() const;
};

