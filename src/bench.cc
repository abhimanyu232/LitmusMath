#include <filesystem>
#include <print>

// user-defined includes
#include "../include/common.h"
#include "../include/timer.h"

// user-defined libraries
#include "../include/matrix.h"
#include "../include/statistics.h"

int main() {

	constexpr size_t n_row = 32 * 2;
	constexpr size_t n_col = 32 * 2;
	constexpr size_t size_array = n_row * n_col;

	// number of iterations to benchmark over.
	const size_t niter = 100;

	// random vector generator
	uint64_t seed = 42;

	// wrapper to generate vector populated with random floats.
	auto getRandomVector = [](float_type min, float_type max,
														uint64_t seed) -> std::vector<float_type> {
		return GetUniformRandomNumbers<size_array>(min, max, seed);
	};

	auto vec_1 = getRandomVector(0, 1, seed);
	Matrix<float_type> fMatrix1(vec_1, n_row, n_col);

	auto vec_2 = getRandomVector(0, 1, seed);
	Matrix<float_type> fMatrix2(vec_2, n_row, n_col);

	// wrapper to benchmark functions
	auto benchmark_function = [](std::string benchmark_id, bool writeBenchToFile,
															 size_t iterations, size_t size_array,
															 auto&& func, auto&&... args) {
		using ResultType = decltype(std::forward<decltype(func)>(func)(
			std::forward<decltype(args)>(args)...));

		ResultType result = std::forward<decltype(func)>(func)(
				std::forward<decltype(args)>(args)...);
		result = result - result;

		std::vector<double> timer_runner;
		Statistics timer_stats;

		for (size_t i = 0; i < iterations; ++i) {
			auto start = timer::GetCurrentTime();
			ResultType temp_result = std::forward<decltype(func)>(func)(
				std::forward<decltype(args)>(args)...);
			auto end = timer::GetCurrentTime();
			auto total_duration = end - start;
			timer_runner.emplace_back(total_duration.count());

			result += temp_result;
		}

		timer_stats.UpdateSummaryStats<double>(timer_runner);

		std::println("{}:\t Size:{}", benchmark_id, size_array);
		std::println("Average time per call: {} seconds", timer_stats.mean);
		std::println("Std. Deviation : {} seconds",
								 std::sqrt(timer_stats.variance));
		std::println("Total time for {} iterations: {} seconds", iterations,
								 timer_stats.sum);

		if (writeBenchToFile) {
			std::filesystem::path benchFile{benchmark_id + ".dat"};
			if (std::FILE * fstream{std::fopen(benchFile.c_str(), "a")}) {
				// std::print(fstream, "File: {}", benchFile.string());	// overload (2)
				std::println(fstream, "{}\t{}\t{}\t{}\t{}", size_array,
										 timer_stats.mean, std::sqrt(timer_stats.variance),
										 timer_stats.variance, timer_stats.sum);
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

	// Register Functions to Benchmark
	auto matMultBase_f = [](const Matrix<float_type>& fMatA,
													const Matrix<float_type>& fMatB) {
		return matMultBasic(fMatA, fMatB);
	};

	auto MatMultTxp_f = [](const Matrix<float_type>& fMatA,
												 const Matrix<float_type>& fMatB) {
		return MatMultTxp(fMatA, fMatB);
	};

	auto MatMultStrassen_f = [](const Matrix<float_type>& fMatA,
															const Matrix<float_type>& fMatB) {
		return MatMultStrassen(fMatA, fMatB);
	};

	auto mattxp_f = [](const Matrix<float_type>& fMatA) {
		return MatrixTransposeNaive(fMatA);
	};

	// whether to write benchmark files
	bool writeStatsFile = true;
	size_t size_min_exp = 2;
	size_t size_max_exp = 10;
	
	Matrix<float_type> result1;
	std::string fname = "MatrixTransposeNaive";
	if (writeStatsFile)
		open_stats_file(fname);
	for (size_t i = size_min_exp; i <= size_max_exp; i++) {
		const size_t size_vec = std::pow(4, i);
		const size_t rows = std::sqrt(size_vec);
		const size_t cols = rows;

		auto vec1 = GetUniformRandomNumbers(size_vec, 0, 1 );
		Matrix<float_type> matA(vec1, rows, cols);
		result1 = benchmark_function(fname, writeStatsFile, niter, size_vec,
																 mattxp_f, matA);
	}
	std::filesystem::path testWriteFile1{"matrixTxpSums.dat"};
	if (std::FILE * fstream{std::fopen(testWriteFile1.c_str(), "w")}) {
		std::println(fstream, "{}", result1);
		std::fclose(fstream);
	}

	Matrix<float_type> result;
	fname = "MatMult_Basic";
	if (writeStatsFile)
		open_stats_file(fname);
	for (size_t i = size_min_exp; i <= size_max_exp; i++) {
		const size_t size_vec = std::pow(4, i);
		const size_t rows = std::sqrt(size_vec);
		const size_t cols = rows;

		auto vec1 = GetUniformRandomNumbers(size_vec, 0, 1 );
		Matrix<float_type> matA(vec1, rows, cols);
		auto vec2 = GetUniformRandomNumbers(size_vec, 0, 1 );
		Matrix<float_type> matB(vec2, rows, cols);

		result = benchmark_function(fname, writeStatsFile, niter, size_vec,
																 matMultBase_f, matA, matB);
	}

	fname = "MatMult_Transpose";
	if (writeStatsFile)
		open_stats_file(fname);
	for (size_t i = size_min_exp; i <= size_max_exp; i++) {
		const size_t size_vec = std::pow(4, i);
		const size_t rows = std::sqrt(size_vec);
		const size_t cols = rows;

		auto vec1 = GetUniformRandomNumbers(size_vec, 0, 1 );
		Matrix<float_type> matA(vec1, rows, cols);
		auto vec2 = GetUniformRandomNumbers(size_vec, 0, 1 );
		Matrix<float_type> matB(vec2, rows, cols);

		result = benchmark_function(fname, writeStatsFile, niter, size_vec,
																 MatMultTxp_f, matA, matB);
	}

	fname = "MatMult_Strassen";
	if (writeStatsFile)
		open_stats_file(fname);
	for (size_t i = size_min_exp; i <= size_max_exp; i++) {
		const size_t size_vec = std::pow(4, i);
		const size_t rows = std::sqrt(size_vec);
		const size_t cols = rows;

		auto vec1 = GetUniformRandomNumbers(size_vec, 0, 1 );
		Matrix<float_type> matA(vec1, rows, cols);
		auto vec2 = GetUniformRandomNumbers(size_vec, 0, 1 );
		Matrix<float_type> matB(vec2, rows, cols);

		result = benchmark_function(fname, writeStatsFile, niter, size_vec,
																 MatMultStrassen_f, matA, matB);
	}
	
	return 0.;
}