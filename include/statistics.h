// utility functions to get basic statistics for input data.
#ifndef STATISTICS_H
#define STATISTICS_H

#include <algorithm>
#include <cmath>
#include <print>
#include <ranges>
#include <span>

#include "../include/common.h"

struct Statistics {
  float_type mean = 0.;
  float_type variance = 0.;
  float_type sample_variance = 0.;
  float_type median = 0;
  float_type geometric_mean = 1;

  /// @brief get standard stats : mean variance and median of any input of contiguous data.
  /// @tparam T
  /// @param data
  template <typename T>
  void UpdateSummaryStats(std::span<T> data) {

	float_type running_sum = 0.;
	float_type running_sum_sqr = 0.;
	for (const auto ele : data) {
	  running_sum += ele;
	  running_sum_sqr += ele * ele;
	}

	mean = running_sum / data.size();

	// todo: should not do this, suffers from catastrophic cancellation
	// https://en.wikipedia.org/wiki/Catastrophic_cancellation
	// https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
	variance = (running_sum_sqr / data.size()) - (mean * mean);

	// unbiased sample variance hence factor of (n/n-1)
	// todo: see above
	sample_variance = (data.size() / (data.size() - 1.0F)) * variance;

	// median
	std::ranges::sort(data);
	auto isEven = [](size_t a) -> bool {
	  return ((a % 2) == 0);
	};
	median = isEven(data.size())
				 ? (data[data.size() / 2] + data[(data.size() / 2) - 1]) / 2.0F
				 : data[std::floor(data.size() / 2)];
	float_type temp = 1.0F;
	for (const auto ele : data) {
	  temp *= ele;
	}
	geometric_mean = std::powl(temp, 1.0F / data.size());
  }

  template <typename T>
  float_type UpdateVariance(std::span<T> data) {
	float_type temp = 0.F;

	return temp;
  }

  // todo: Compute Mode
  // template <typename T>
  // float_type ComputeMode(std::span<T> data) {
  // 	return 0.;
  // }

  template <typename T>
  float_type UpdateMedian(std::span<T> data) {
	std::ranges::sort(data);
	auto isEven = [](int a) -> bool {
	  return ((a % 2) == 0);
	};
	median = isEven(data.size())
				 ? (data[data.size() / 2] + data[-1 + data.size() / 2]) / 2.
				 : data[std::floor(data.size() / 2)];

	return median;
  }

  template <typename T>
  float_type UpdateGeometricMean(std::span<T> data) {

	float_type temp = 1.0F;
	for (const auto ele : data) {
	  temp *= ele;
	}
	geometric_mean = std::powl(temp, 1.0F / data.size());
	return geometric_mean;
  }

  /// @brief print all the computed statistics
  void PrintSummaryStats() {
	std::println(
		" Mean: {:.10f} \n Median: {:.10f} \n Variance: {:.10f} \n Sample "
		"Variance: {:.10f} \n Geo "
		"Mean: {:.10f}",
		mean, median, variance, sample_variance, geometric_mean);
  }

  /// @brief reset all the stats to 0.
  void ResetSummaryStats() {
	mean = 0.;
	geometric_mean = 0.;
	variance = 0.;
	sample_variance = 0.;
	median = 0.;
  }
};

#endif