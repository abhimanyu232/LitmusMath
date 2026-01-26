#ifndef COMMON_H
#define COMMON_H

#include <random>

using float_type = double;

// values 

/** @brief value of pi */
template<typename T>
constexpr T pi{ 3.1415926535897932385 };

// todo:
template <typename T>
constexpr T KahanSum(const T& a, const T& b) {
	return a + b;
}

template <typename T>
constexpr T PairwiseSum(const T& a, const T& b) {
	return a + b;
}

/**
/// @brief get n uniformly distributed random floating point numbers in range [min,max)
/// @param n: number of random numbers to generate
/// @param min,max : range of random numbers. default [0,1)
/// @param seed: seed for random number generator, default: std::random_device
/// @return std::vector<float_type> of size size
*/
template <size_t SIZE>
inline std::vector<float_type> GetUniformRandomNumbers(
	float_type min = 0.F, float_type max = 1.0F,
	uint64_t seed = std::random_device{}()) {
	std::mt19937_64 rng(seed);	// Mersenne Twister 64-bit
	std::uniform_real_distribution<float_type> dist(min, max);	// [0,1)

	std::vector<float_type> out(SIZE);
	for (std::size_t i = 0; i < SIZE; ++i) {
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
template <size_t SIZE>
inline std::vector<float_type> GetNormalRandomNumbers(
	float_type mean = 0.F, float_type std_dev = 1.0F,
	uint64_t seed = std::random_device{}()) {
	std::mt19937_64 rng(seed);
	std::normal_distribution<float_type> dist(mean, std_dev);

	std::vector<float_type> out(SIZE);
	for (std::size_t i = 0; i < SIZE; ++i) {
		out[i] = dist(rng);
	}

	return out;
}

#endif