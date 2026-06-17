// user-defined includes
#include "common.h"

// user-defined libraries
#include "../libs/serial/matrix.h"
#include "../libs/common/statistics.h"
#include "../libs/common/timer.h"

using namespace matrix_serial;

int main() {

	std::vector<int> Vec1{1, 1, 2, 2, 3, 3};
	Matrix<int> Matrix1(Vec1, 3, 2);
	std::print("{}", Matrix1);

	std::vector<float> Vec2{1.2, 2.3, 3.4, 4.5, 5.6, 6.7};
	Matrix<float> Matrix2(Vec2, 2, 3);
	std::print("{}", Matrix2);

	auto cStyle_result = cStyle_matMultBasic<int, float>(
		Vec1, std::make_pair(3, 2), Vec2, std::make_pair(2, 3));
	/** for (auto ele : cStyle_result) {
	*	  	std::println("{}", ele);
	*   }	*/

	std::println("A*B");
	auto result = matMultBasic(Matrix1, Matrix2);
	std::print("{}", result);
	// correct result
	// {{5.7, 7.9, 10.1},
	// {11.4, 15.8, 20.2},
	// {17.1, 23.7, 30.3}}

	std::println("A*B (using transposed B)");
	auto resultof_ABTxp = MatMultTxp(Matrix1, Matrix2);
	std::print("{}", resultof_ABTxp);

	std::println("B*A");
	auto result21 = matMultBasic(Matrix2, Matrix1);
	std::print("{}", result21);

	std::println("B*A (using transposed A)");
	auto resultof_BATxp = MatMultTxp(Matrix2, Matrix1);
	std::print("{}", resultof_BATxp);
	// correct result
	// 16	16
	// 35.8	35.8

	/* Matrix Comparison */

	auto equal = (Matrix2 == result21);
	if (equal)	std::println("matrix equal");
	else  std::println("matrix not equal");

	Matrix<int> int_Matrix23(Vec1, 2, 3);

	Matrix2 = result21;

	equal = (Matrix2 <= result21);
	if (equal)	std::println("matrix2 less equal");
	else	std::println("matrix2 greater than");

	/* Matrix conversion */

	Matrix<int> intMatrixToConvert(Vec1, 3, 2);
	auto convertedToFloatFromInt = static_cast<Matrix<float>>(intMatrixToConvert);
	std::println("{}", convertedToFloatFromInt);

	Matrix<float> floatMatrixToConvert(Vec2, 3, 2);
	// !!! illegal operation. narrowing conversion
	// auto convertedToIntFromFromFloat = static_cast<Matrix<int>>(floatMatrixToConvert);

	/* Random Number Generation */
	std::println("Gaussian Random  ");
	auto normal_random = GetNormalRandomNumbers<100>(0., 1.0, 42);
	for (size_t i = 0; i < normal_random.size(); ++i) {
		std::println("{}, {:.6f},", i, normal_random[i]);
	}

	std::println("Unifrom Random  ");
	constexpr size_t size44 = 16;
	auto uniform_random16 = [](float_type min, float_type max,
														 uint64_t seed) -> std::vector<float_type> {
		return GetUniformRandomNumbers<size44>(min, max, seed);
	};

	return 0;
}
