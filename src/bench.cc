#include <print>
// user-defined includes
#include "../include/common.h"
#include "../include/timer.h"

// user-defined libraries
#include "../include/matrix.h"
#include "../include/statistics.h"

int main() {

	constexpr size_t n_row = 150;
	constexpr size_t n_col = 150;
	constexpr size_t size_array = n_row * n_col;

	auto getRandomVector = [](float_type min, float_type max,
														uint64_t seed) -> std::vector<float_type> {
		return GetUniformRandomNumbers<size_array>(min, max, seed);
	};

	// generate random 4x4 array
	auto arr_1 = getRandomVector(0, 1, 42);
	Matrix<float_type> fMatrix1(
		std::vector<float_type>(std::move_iterator(arr_1.begin()),
														std::move_iterator(arr_1.end())),
		n_row, n_col);

	auto arr_2 = getRandomVector(0, 1, 42);
	Matrix<float_type> fMatrix2(
		std::vector<float_type>(std::move_iterator(arr_2.begin()),
														std::move_iterator(arr_2.end())),
		n_row, n_col);

	// wrapper to benchmark functions
	auto benchmark_function = [](size_t iterations, auto&& func, auto&&... args) {
		using ResultType = decltype(std::forward<decltype(func)>(func)(
			std::forward<decltype(args)>(args)...));
		ResultType result;

		auto start = timer::GetCurrentTime();
		for (size_t i = 0; i < iterations; ++i) {
			result = std::forward<decltype(func)>(func)(
				std::forward<decltype(args)>(args)...);
		}
		auto end = timer::GetCurrentTime();

		auto total_duration = end - start;
		auto avg_duration = total_duration / static_cast<double>(iterations);
		std::println("Total time for {} iterations: {} seconds", iterations,
								 total_duration.count());
		std::println("Average time per call: {} seconds", avg_duration.count());
		return result;
	};

	// matrix op wrappers
	auto matMultBase_f = [](const Matrix<float_type>& fMatA,
													const Matrix<float_type>& fMatB) {
		return matMultBasic(fMatA, fMatB);
	};

	auto MatMultTxp_f = [](const Matrix<float_type>& fMatA,
												 const Matrix<float_type>& fMatB) {
		return MatMultTxp(fMatA, fMatB);
	};

	auto mattxp_f = [](const Matrix<float_type>& fMatA) {
		return MatrixTransposeNaive(fMatA);
	};

	// number of iterations to benchmark over.
	const size_t niter = 100;

	std::println("MatrixTransposeNaive");
	auto result_matTxp = benchmark_function(niter, mattxp_f, fMatrix2);

	std::println("matMultBasic");
	auto result_matMultBasic =
		benchmark_function(niter, matMultBase_f, fMatrix1, fMatrix2);

	std::println("MatMultTxp");
	auto result_MatMultTxp =
		benchmark_function(niter, MatMultTxp_f, fMatrix1, fMatrix2);

	return 0.;
}