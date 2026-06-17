#ifndef COMMON_H
#define COMMON_H

#include <print>

#include <random>

#include <concepts>
#include <span>
#include <vector>

// commmon math functions
#include <cmath>
#include <cstdlib>
#include <numbers>

// type alias
using float_type = double;
using index_t = uint32_t;

template <typename T>
concept NumericType = requires(T param) {
	requires std::is_integral_v<T> || std::is_floating_point_v<T>;
	requires !std::is_same_v<bool, T>;
	requires !std::is_pointer_v<T>;
};


// some mathematical constants
constexpr double PI = std::numbers::pi_v<double>;
constexpr double INV_PI = std::numbers::inv_pi_v<double>;
constexpr double INV_SQRT_PI = std::numbers::inv_sqrtpi_v<double>;

constexpr double SQRT_2 = std::numbers::sqrt2_v<double>;

constexpr double SQRT_3 = std::numbers::sqrt3_v<double>;
constexpr double INV_SQRT_3 = std::numbers::inv_sqrt3_v<double>;

constexpr double LOG10_E = std::numbers::log10e_v<double>;
constexpr double LOG2_E = std::numbers::log2e_v<double>;

constexpr double LN_2 = std::numbers::ln2_v<double>;
constexpr double LN_10 = std::numbers::ln10_v<double>;


// Convenience Functions

// check for evenness of integer types
template <typename T>
	requires(std::integral<T>)
constexpr bool isEven(T a) {
	// bitshift instead of this?
	return ((a % 2) == 0);
}

/**
/// @brief Calculates the next power of two greater than or equal to input
/// @tparam T : type of input
/// @param input : some number
/// @return value: input if it is a power of two else the next largest power of two 
*/
template <typename T>
constexpr auto GetNextPowerOfTwo(T input) {
	if (input == 0)
		return T(1);
	T power = 1;
	while (power < input)
		power <<= 1;
	return power;
}

//!!! This uses three double math functions (log2, floor, pow) for a purely integer operation.
//!!! For large integers (> 2^53), precision loss causes incorrect results. use bit manipulation here
// template <typename T>
// constexpr auto GetNextPowerOfTwo(T input) {
// 	return (std::floor(std::log2(input)) == std::log2(input))
// 					 ? input
// 					 : std::pow(2, std::floor(std::log2(input)) + 1);
// }

//!!! Ambiguous overloads for GetUniformRandomNumbers and GetNormalRandomNumbers
//!!! why is SIZE a template parameter.


// high precision floating point summation

/**
 * @brief Kahan Summation (Compensated summation)
 * 
 * For summation large sequence of floats with reduced round-off error
 * 
 * Cost : 4 Flops 
 *  
 * @tparam T : floating point type
 * @param a : type convertible to span
 */
template <typename T>
	requires(std::floating_point<T>)
[[nodiscard]] constexpr T KahanSum(std::span<const T> a) {
	index_t size = a.size();
	T sum = 0;
	T comp = 0;
	for (index_t idx = 0; idx < size; ++idx) {
		T temp_y = a[idx] - comp;
		T temp_t = sum + temp_y;
		// note: careful! -ffast-math/-fassociative-math will break kahan and derivatives.
		comp = (temp_t - sum) - temp_y;
		sum = temp_t;
	}
	return sum;
}

/**
 * @brief  Kahan Neumaier Sum (Kahan–Babuška)
 * 
 * Higher precision variant of Kahan Sum
 * 
 * Cost : 4 Flops + branch prediction cost
 * 
 * @tparam T : floating point type
 * @param a : type convertible to span
 */
template <typename T>
	requires(std::floating_point<T>)
[[nodiscard]] constexpr T KahanNeumaierSum(std::span<const T> a) {
	index_t size = a.size();
	T sum = 0;
	T comp = 0;
	for (index_t idx = 0; idx < size; ++idx) {
		T temp_t = a[idx] + sum;
		//!!! branch mis-prediction cost 15-20 cycles
		if (std::abs(sum) >= std::abs(a[idx])) {
			// If sum is bigger, low-order digits of input[i] are lost.
			comp += sum - temp_t + a[idx];
		} else {
			// Else low-order digits of sum are lost.
			comp += (a[idx] - temp_t) + sum;
		}
		sum = temp_t;
	}
	return sum + comp;
}

// todo: branchless kahan using Knuth/Dekker2Sum instead of fast2sum

/**
 * @brief Pairwise Summation
 * 
 * Natively Parallelizable 
 * 
 * @tparam T 
 */
template <NumericType T>
[[nodiscard]] T PairwiseSum(std::span<const T> a) {
	const index_t size = a.size();

	if (size == 0)
		return T{0};
	if (size == 1)
		return a[0];

	// check for non-even size and handle

	// temp heap vector of half size, enough to fit in temp sums
	std::vector<T> buffer(a.begin(), a.end());

	index_t n = size;
	while (n > 1) {
		const index_t half = n / 2;

#pragma omp parallel for if (half >= 128)
		for (index_t idx = 0; idx < half; ++idx) {
			buffer[idx] = buffer[idx] + buffer[half + idx];
			// could also be a[size-idx-1]
			// a[0] = a[0] + a[size - 1];
			// a[1] = a[1] + a[size - 2];
			// ...
			// a[half - 1] = a[(size / 2) - 1] + a[(size / 2)];
		}

		// add the last value in the current set
		buffer[0] = (!isEven(n)) ? buffer[0] + buffer[n - 1] : buffer[0];

		n = half;
	}

	return buffer[0];
}

// Random number generator helpers

/**
/// @brief get n uniformly distributed random floating point numbers in range [min,max)
/// @param n: number of random numbers to generate
/// @param min,max : range of random numbers. default [0,1)
/// @param seed: seed for random number generator, default: std::random_device
/// @return std::vector<float_type> of size size
*/
template <index_t SIZE>
inline std::vector<float_type> GetUniformRandomNumbers(
	float_type min = 0., float_type max = 1.0,
	uint64_t seed = std::random_device{}()) {
	std::mt19937_64 rng(seed);	// Mersenne Twister 64-bit
	std::uniform_real_distribution<float_type> dist(min, max);	// [0,1)

	std::vector<float_type> out(SIZE);
	for (index_t i = 0; i < SIZE; ++i) {
		out[i] = dist(rng);
	}

	return out;
}

/**
/// @brief get n uniformly distributed random floating point numbers in range [min,max)
/// @param n: number of random numbers to generate
/// @param min,max : range of random numbers. default [0,1)
/// @param seed: seed for random number generator, default: std::random_device
/// @return std::vector<float_type> of size size
*/
inline std::vector<float_type> GetUniformRandomNumbers(
	const index_t SIZE = 1, float_type min = 0., float_type max = 1.0,
	uint64_t seed = std::random_device{}()) {
	std::mt19937_64 rng(seed);	// Mersenne Twister 64-bit
	std::uniform_real_distribution<float_type> dist(min, max);	// [0,1)

	std::vector<float_type> out(SIZE);
	for (index_t i = 0; i < SIZE; ++i) {
		out[i] = dist(rng);
	}

	return out;
}

/** 
/// @brief get n normally distributed/gaussian random floating point numbers with (mean,standard deviation)
/// @param n: number of random numbers to generate
/// @param mean,std_dev : mean and standard deviation of gaussian sample, default (0,1)
/// @param seed: seed for random number generator, default: std::random_device
/// @return std::vector<float_type> of size SIZE
*/
template <index_t SIZE>
inline std::vector<float_type> GetNormalRandomNumbers(
	float_type mean = 0., float_type std_dev = 1.0,
	uint64_t seed = std::random_device{}()) {
	std::mt19937_64 rng(seed);
	std::normal_distribution<float_type> dist(mean, std_dev);

	std::vector<float_type> out(SIZE);
	for (index_t i = 0; i < SIZE; ++i) {
		out[i] = dist(rng);
	}

	return out;
}

/** 
/// @brief get n normally distributed/gaussian random floating point numbers with (mean,standard deviation)
/// @param n: number of random numbers to generate
/// @param mean,std_dev : mean and standard deviation of gaussian sample, default (0,1)
/// @param seed: seed for random number generator, default: std::random_device
/// @return std::vector<float_type> of size SIZE
*/
inline std::vector<float_type> GetNormalRandomNumbers(
	const index_t SIZE = 1, float_type mean = 0., float_type std_dev = 1.0,
	uint64_t seed = std::random_device{}()) {
	std::mt19937_64 rng(seed);
	std::normal_distribution<float_type> dist(mean, std_dev);

	std::vector<float_type> out(SIZE);
	for (index_t i = 0; i < SIZE; ++i) {
		out[i] = dist(rng);
	}

	return out;
}

#endif