// basic utility functions and macros to time code
#ifndef LOCAL_TIMER_H
#define LOCAL_TIMER_H

#include <chrono>

#include <filesystem>
#include <print>

/// user-defined includes
#include "statistics.h"
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

namespace benchmark {

auto benchmark_function = [](std::string benchmark_id, bool writeBenchToFile,
														 size_t iterations, size_t size_array, auto&& func,
														 auto&&... args) {
	using ResultType = decltype(std::forward<decltype(func)>(func)(
		std::forward<decltype(args)>(args)...));
	ResultType result;

	std::vector<double> timer_runner;
	Statistics timer_stats;

	for (size_t i = 0; i < iterations; ++i) {
		auto start = GetCurrentTime();
		result =
			std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...);
		auto end = GetCurrentTime();
		auto total_duration = end - start;
		timer_runner.emplace_back(total_duration.count());
	}

	timer_stats.UpdateSummaryStats<double>(timer_runner);

	std::println("{}:,\t Size:{}", benchmark_id, size_array);
	std::println("Average time per call: {} seconds", timer_stats.mean);
	std::println("Std. Deviation : {} seconds", std::sqrt(timer_stats.variance));
	std::println("Total time for {} iterations: {} seconds", iterations,
							 timer_stats.sum);

	if (writeBenchToFile) {
		std::filesystem::path benchFile{benchmark_id + ".dat"};
		if (std::FILE * fstream{std::fopen(benchFile.c_str(), "a")}) {
			// std::print(fstream, "File: {}", benchFile.string());	// overload (2)
			std::println(fstream, "{}\t{}\t{}\t{}\t{}", size_array, timer_stats.mean,
									 std::sqrt(timer_stats.variance), timer_stats.variance,
									 timer_stats.sum);
			std::fclose(fstream);
		}
	}

	return result;
};

// wrapper to open file to write benchmark stats to
auto open_stats_file = [](std::string fname) {
	std::filesystem::path benchFile{fname + ".dat"};
	if (std::FILE * fstream{std::fopen(benchFile.c_str(), "w")}) {
		std::println(fstream,
								 "size\tmean_time(s)\tstd_dev\tvariance\ttotal_time(s)");
		std::fclose(fstream);
	}
};
}	 // namespace benchmark

}	 // namespace timer

#endif