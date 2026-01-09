// basic utility functions and macros to time code
#ifndef LOCAL_TIMER_H
#define LOCAL_TIMER_H

#include <chrono>

// #include <thread>
// #include <print>

/// user-defined includes
// #include "statistics.h"
namespace timer {
using namespace std::chrono_literals;
using Time = std::chrono::steady_clock;
// using Time = std::chrono::high_resolution_clock;
// can use ::system_clock which is not guaranteed to be monotonic.
// hence, prefer ::steady_clock.

using DoubleSeconds = std::chrono::duration<double>;
using FloatTimePoint = std::chrono::time_point<Time, DoubleSeconds>;

// constexpr
FloatTimePoint getCurrentTime() {
  return Time::now();
}
}  // namespace timer

#endif

// int main() {

// 	double total_timer;
// 	for (uint16_t i = 0; i < 1000; ++i) {
// 		auto startTime = getCurrentTime();
// 		std::this_thread::sleep_for(5ms);
// 		auto endTime = getCurrentTime();
// 		total_timer += (endTime - startTime).count();
// 		std::println("invocation time: {} \t {} ", i, endTime - startTime);
// 	}

// 	std::println(" average time: {} ", total_timer / 1000);
// }