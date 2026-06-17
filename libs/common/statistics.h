// utility functions to get basic statistics for input data.
#ifndef STATISTICS_H
#define STATISTICS_H

#include <algorithm>
#include <ranges>

// user-defined includes
#include "common.h"

struct Statistics {
	float_type sum = 0;
	float_type mean = 0.;
	float_type variance = 0.;
	float_type sample_variance = 0.;
	float_type median = 0;
	float_type geometric_mean = 1;

	Statistics() {}

	template <typename T>
	Statistics(std::span<T> in_data) {
		UpdateSummaryStats(in_data);
	}

	/// @brief get standard stats : mean variance and median of any input of contiguous data.
	/// @tparam T
	/// @param data
	template <typename T>
	void UpdateSummaryStats(std::span<T> data) {

		// static check to ensure data.size() > 1 

		float_type running_sum_sqr = 0.;
		float_type log_sum = 0.;
		for (const auto ele : data) {
			sum += ele;
			running_sum_sqr += ele * ele;
			log_sum += std::log(static_cast<float_type>(ele));
		}

		mean = sum / data.size();
		geometric_mean = std::exp(log_sum/data.size());

		float_type deviation{0.}; 
		for (const auto ele : data){ 
			float_type temp_dev = static_cast<float_type>(ele - mean); 
			deviation += temp_dev*temp_dev;
		}
		variance = deviation/data.size();
		sample_variance = deviation/(data.size()-1);

		// note: dont do this:  
		// variance = (running_sum_sqr / data.size()) - (mean * mean);
		// https://en.wikipedia.org/wiki/Catastrophic_cancellation
		// https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
		

		// median		
		// note: using std::nth_element instead is O(n) instead of O(nlogn) for std::sort
		// only sorts until the mid-point which is needed here. 
		// also destructive
		std::ranges::sort(data);	// destructive sort
		// note: moving the log_sum after this and using O2, the geometric mean  was computed wrong
		// probably a compiler optimisation where it would lift it before the sort, 
		// and then read stale values. possibly

		auto isEven = [](size_t a) -> bool {
			return ((a % 2) == 0);
		};
		median = isEven(data.size())
							 ? (data[data.size() / 2] + data[(data.size() / 2) - 1]) / 2.0
							 : data[std::floor(data.size() / 2)];
	}

	// template <typename T>
	// float_type UpdateVariance(std::span<T> data) {
	// float_type temp = 0.;
	// //!!! not implemented
	// return temp;
	// }

	// todo: Compute Mode
	// template <typename T>
	// float_type ComputeMode(std::span<T> data) {
	// 	return 0.;
	// }

	template <typename T>
	[[nodiscard]] float_type UpdateMedian(std::span<T> data) {
		std::ranges::sort(data);	//!!! destructive sort, modifies underlying data
		auto isEven = [](size_t a) -> bool {
			return ((a % 2) == 0);
		};
		median = isEven(data.size())
							 ? (data[data.size() / 2] + data[-1 + data.size() / 2]) / 2.
							 : data[std::floor(data.size() / 2)];

		return median;
	}

	template <typename T>
	[[nodiscard]] float_type UpdateGeometricMean(std::span<T> data) {
		float_type log_sum = 0; 

		// log-sum-exp to avoid overflow resulting from float-point mult error 
		for (const auto ele : data){
			log_sum += std::log(static_cast<float_type>(ele));
		}
		geometric_mean = std::exp(log_sum/data.size());

		return geometric_mean;
	}

	/// @brief print all the computed statistics
	void PrintSummaryStats() const noexcept {
		std::println(
			" Mean: {:.10f} \n Median: {:.10f} \n Variance: {:.10f} \n Sample "
			"Variance: {:.10f} \n Geo "
			"Mean: {:.10f}",
			mean, median, variance, sample_variance, geometric_mean);
	}

	/// @brief reset all the stats to 0.
	void ResetSummaryStats() {
		sum = 0.;
		mean = 0.;
		geometric_mean = 0;
		variance = 0.;
		sample_variance = 0.;
		median = 0.;
	}
};

#endif