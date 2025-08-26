#include "Timer.h"

using namespace std;

Timer::Timer(double maxSeconds) : maxSeconds(maxSeconds), startTime(steady_clock::now()) {}

bool Timer::isRunning() const
{
    return getRemainingTime() > 0.0;
}

double Timer::getRemainingTime() const
{
    auto now = steady_clock::now();
    double elapsed = duration<double>(now - startTime).count();
    return std::max(0.0, maxSeconds - elapsed);
}
