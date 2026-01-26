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

constexpr FloatTimePoint GetCurrentTime() {
  return Time::now();
}
}  // namespace timer

#endif