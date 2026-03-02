#include <filesystem>
#include <print>

// user-defined includes
#include "../include/common.h"
#include "../include/timer.h"

// user-defined libraries
#include "../include/matrix.h"
#include "../include/statistics.h"

int main(int argc, char** argv) {

	/**
	 * Benchmark settings
	 * size, number of iterations, whether to write to file 
	*/

	// defaults
	size_t size_min_exp = 2;
	size_t size_max_exp = 9;
	size_t niter = 100;

	if (argc == 4) {
		size_min_exp = std::atoi(argv[0]);
		size_max_exp = std::atoi(argv[1]);
		niter = std::atoi(argv[2]);
	} else {
		std::print(
			"no or incorrect command line options chosen, keeping defaults \n");
		std::print(
			"NOTE: command line usage: ./LitmusMath_bench <min_pow_2_size> "
			"<max_pow_2_size> <n_iter> \n");
		std::println();
	}

	bool writeStatsFile = true;	 // whether to write benchmark files

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

	Matrix<float_type> result1;
	std::string fname = "MatrixTransposeNaive";
	if (writeStatsFile)
		timer::benchmark::open_stats_file(fname);
	for (size_t i = size_min_exp; i <= size_max_exp; i++) {
		const size_t size_vec = std::pow(4, i);
		const size_t rows = std::sqrt(size_vec);
		const size_t cols = rows;

		auto vec1 = GetUniformRandomNumbers(size_vec, 0, 1);
		Matrix<float_type> matA(vec1, rows, cols);

		result1 = timer::benchmark::benchmark_function(fname, writeStatsFile, niter,
																									 size_vec, mattxp_f, matA);

		if (result1.size() == size_t(GetUniformRandomNumbers()[0]))
			result1 += result1;
	}

	Matrix<float_type> result;
	fname = "MatMult_Basic";

	if (writeStatsFile)
		timer::benchmark::open_stats_file(fname);
	for (size_t i = size_min_exp; i <= size_max_exp; i++) {
		const size_t size_vec = std::pow(4, i);
		const size_t rows = std::sqrt(size_vec);
		const size_t cols = rows;

		auto vec1 = GetUniformRandomNumbers(size_vec, 0, 1);
		Matrix<float_type> matA(vec1, rows, cols);
		auto vec2 = GetUniformRandomNumbers(size_vec, 0, 1);
		Matrix<float_type> matB(vec2, rows, cols);

		result = timer::benchmark::benchmark_function(
			fname, writeStatsFile, niter, size_vec, matMultBase_f, matA, matB);

		if (result.size() == size_t(GetUniformRandomNumbers()[0]))
			result += result;
	}

	fname = "MatMult_Transpose";
	if (writeStatsFile)
		timer::benchmark::open_stats_file(fname);
	for (size_t i = size_min_exp; i <= size_max_exp; i++) {
		const size_t size_vec = std::pow(4, i);
		const size_t rows = std::sqrt(size_vec);
		const size_t cols = rows;

		auto vec1 = GetUniformRandomNumbers(size_vec, 0, 1);
		Matrix<float_type> matA(vec1, rows, cols);
		auto vec2 = GetUniformRandomNumbers(size_vec, 0, 1);
		Matrix<float_type> matB(vec2, rows, cols);

		result = timer::benchmark::benchmark_function(
			fname, writeStatsFile, niter, size_vec, MatMultTxp_f, matA, matB);

		if (result.size() == size_t(GetUniformRandomNumbers()[0]))
			result += result;
	}

	fname = "MatMult_Strassen";
	if (writeStatsFile)
		timer::benchmark::open_stats_file(fname);
	for (size_t i = size_min_exp; i <= size_max_exp; i++) {
		const size_t size_vec = std::pow(4, i);
		const size_t rows = std::sqrt(size_vec);
		const size_t cols = rows;

		auto vec1 = GetUniformRandomNumbers(size_vec, 0, 1);
		Matrix<float_type> matA(vec1, rows, cols);
		auto vec2 = GetUniformRandomNumbers(size_vec, 0, 1);
		Matrix<float_type> matB(vec2, rows, cols);

		result = timer::benchmark::benchmark_function(
			fname, writeStatsFile, niter, size_vec, MatMultStrassen_f, matA, matB);

		if (result.size() == size_t(GetUniformRandomNumbers()[0]))
			result += result;
	}

	return 0.;
}