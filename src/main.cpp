#include <iostream> // // import <iostream>;
#include "../include/matrix.h"
// #include <gtest/gtest.h>

int main() {
	// constexpr size_t size = 6;
	std::vector<int> Vec1{1, 1, 2, 2, 3, 3};
	Matrix<int> Matrix1(Vec1, 3, 2);

	std::vector<float> Vec2{1.2, 2.3, 3.4, 4.5, 5.6, 6.7};
	Matrix<float> Matrix2(Vec2, 2, 3);

	Matrix1.print_matrix();
	Matrix2.print_matrix();

	auto result = matMultBasic(Matrix1, Matrix2);
	result.print_matrix();
    // correct result
    // {{5.7, 7.9, 10.1}, 
	// {11.4, 15.8, 20.2}, 
	// {17.1, 23.7, 30.3}}

	auto cStyle_result =
			cStyle_matMultBasic<int,float>(Vec1, std::make_pair(3, 2),Vec2, std::make_pair(2, 3));
	for (auto ele : cStyle_result)
		std::cout << ele  << std::endl; 

	return 0;
}
