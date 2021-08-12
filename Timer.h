#pragma once
#ifndef CHILI_TIMER_H
#define CHILI_TIMER_H

#include <chrono>
using namespace std::chrono;

class Timer
{
public:
	Timer() noexcept;
	float Mark() noexcept;
	float Peek() const noexcept;
private:
	steady_clock::time_point last;
};
#endif // !CHILI_TIMER_H

