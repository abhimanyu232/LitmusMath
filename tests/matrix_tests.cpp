#include <gtest/gtest.h>
#include <cmath>
#include <print>
#include "../include/common.h"
#include "../include/matrix.h"

class MatrixTest : public testing::Test {
 protected:
	MatrixTest()
			: iMatrix1(iVec1, 4, 3),
				iMatrix2(iVec2, 3, 4),
				fMatrix1(fVec1, 2, 3),
				fMatrix2(fVec2, 3, 2) {}

	// helper vectors
	const size_t i_vector_size = 12;
	std::vector<int> iVec1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	std::vector<int> iVec2{13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};

	const size_t f_vector_size = 6;
	std::vector<float_type> fVec1{1.2, 2.3, 3.4, 4.5, 5.6, 6.7};
	std::vector<float_type> fVec2{7.8, 8.9, 9.10, 10.11, 11.12, 12.13};

	// int matrices
	Matrix<int> iMatrix1;
	Matrix<int> iMatrix2;

	// floating point matrices
	Matrix<float_type> fMatrix1;
	Matrix<float_type> fMatrix2;
};

TEST_F(MatrixTest, MakeEmptyMatrix) {

	// empty matrix, no size, only size reserved.
	Matrix<int> emptyMatrix1;
	EXPECT_TRUE(emptyMatrix1.size() == 0);
	EXPECT_TRUE(emptyMatrix1.size() != 16);
	EXPECT_TRUE(emptyMatrix1.elements().capacity() == 16);

	// null matrix with given shape
	constexpr size_t empty_matrix_m = 12;
	constexpr size_t empty_matrix_n = 9;
	constexpr size_t empty_matrix_size = empty_matrix_m * empty_matrix_n;

	Matrix<float_type> emptyMatrix2(empty_matrix_m, empty_matrix_n);
	EXPECT_TRUE(emptyMatrix2.size() == empty_matrix_size);
	EXPECT_TRUE(emptyMatrix2.shape().first == empty_matrix_m);
	EXPECT_TRUE(emptyMatrix2.shape().second == empty_matrix_n);
	for (const auto& ele : emptyMatrix2.elements()) {
		EXPECT_TRUE(ele == 0.F);
	}
}

TEST_F(MatrixTest, ConstructFromVectors) {
	// construct from int vector
	EXPECT_TRUE(iMatrix1.size() == 12);
	EXPECT_TRUE(iMatrix1.shape().first == 4);
	EXPECT_TRUE(iMatrix1.shape().second == 3);

	// construct from float vector
	EXPECT_TRUE(fMatrix1.size() == 6);
	EXPECT_TRUE(fMatrix1.shape().first == 2);
	EXPECT_TRUE(fMatrix1.shape().second == 3);
}

TEST_F(MatrixTest, ConstructFromMatrix) {
	// copy constructors

	// copy int from int
	Matrix<int> copy_imatrix1(iMatrix1);
	EXPECT_TRUE(copy_imatrix1.size() == 12);
	EXPECT_TRUE(copy_imatrix1.shape().first == 4);
	EXPECT_TRUE(copy_imatrix1.shape().second == 3);
	for (size_t i = 0; i < copy_imatrix1.size(); ++i) {
		EXPECT_TRUE(copy_imatrix1[i] == iMatrix1[i]);
	}

	// copy assign
	Matrix<int> copy_imatrix2 = iMatrix1;
	EXPECT_TRUE(copy_imatrix2.size() == 12);
	EXPECT_TRUE(copy_imatrix2.shape().first == 4);
	EXPECT_TRUE(copy_imatrix2.shape().second == 3);
	for (size_t i = 0; i < copy_imatrix2.size(); ++i) {
		EXPECT_TRUE(copy_imatrix2[i] == iMatrix1[i]);
	}

	//!!! copying to integral from float point is illegal : narrowing conversion
	// Matrix<int> copy_imatrix3(fMatrix1);
}

TEST_F(MatrixTest, MatrixSimpleArithmetic) {
	// set-up test matrices
	Matrix<float_type> temp_f_Matrix(
		std::vector<float_type>{0.1, 0.2, 0.3, 0.4, 0.5, 0.6}, 2, 3);
	Matrix<int> temp_i_Matrix(std::vector<int>{100, 200, 300, 400, 500, 600}, 2,
														3);

	// add int to float matrix
	temp_f_Matrix += temp_i_Matrix;
	Matrix<float_type> temp_result1(
		std::vector<float_type>{100.1, 200.2, 300.3, 400.4, 500.5, 600.6}, 2, 3);
	for (size_t i = 0; i < temp_f_Matrix.size(); ++i) {
		EXPECT_TRUE(temp_f_Matrix[i] == temp_result1[i]);
	}

	// add float to int matrix
	temp_i_Matrix += temp_f_Matrix;
	Matrix<int> temp_result2(std::vector<int>{200, 400, 600, 800, 1000, 1200}, 2,
													 3);
	for (size_t i = 0; i < temp_i_Matrix.size(); ++i) {
		EXPECT_TRUE(temp_i_Matrix[i] == temp_result2[i]);
	}

	// add different types : result is std::common_type<U,V> -> here : float
	// addition is hence commutative
	auto temp_sum_matrix1 = temp_f_Matrix + temp_i_Matrix;
	auto temp_sum_matrix2 = temp_i_Matrix + temp_f_Matrix;
	Matrix<float_type> temp_sum_matrix_result(
		std::vector<float_type>{300.1, 600.2, 900.3, 1200.4, 1500.5, 1800.6}, 2, 3);
	for (size_t i = 0; i < temp_i_Matrix.size(); ++i) {
		EXPECT_TRUE(temp_sum_matrix1[i] == temp_sum_matrix_result[i]);
		EXPECT_TRUE(temp_sum_matrix2[i] == temp_sum_matrix_result[i]);
	}
}

// TEST_F(MatrixTest, MatrixManipulation) {

// 	auto transposed_int_matrix = MatrixTransposeNaive(iMatrix1);
// 	for (size_t i = 0; i < iMatrix1.size(); ++i) {
// 		// EXPECT_EQ();
// 	}
// 	auto transposed_float_matrix = MatrixTransposeNaive(fMatrix1);
// 	// todo: add test statements
// }

TEST_F(MatrixTest, MatrixMultiplicationNaive) {

	// multiply int matrices
	Matrix<int> matrix_matmult_ivec12 =
		matMultBasic(iMatrix1, iMatrix2);	 // 4x3 * 3x4
	std::vector<int> result_matmult_ivec12{110, 116, 122, 128, 263, 278,
																				 293, 308, 416, 440, 464, 488,
																				 569, 602, 635, 668};	 // 4x4

	EXPECT_TRUE(matrix_matmult_ivec12.size() ==
							result_matmult_ivec12.size());	// == 16
	for (size_t i = 0; i < matrix_matmult_ivec12.size(); ++i) {
		EXPECT_TRUE(matrix_matmult_ivec12[i] == result_matmult_ivec12[i]);
	}

	// multiply int matrices
	Matrix<int> matrix_matmult_ivec21 =
		matMultBasic(iMatrix2, iMatrix1);	 // 3x4 * 4x3
	std::vector<int> result_matmult_ivec21{334, 392, 450, 422, 496,
																				 570, 510, 600, 690};	 // 3x3

	EXPECT_TRUE(matrix_matmult_ivec21.size() ==
							result_matmult_ivec21.size());	// == 9
	for (size_t i = 0; i < matrix_matmult_ivec21.size(); ++i) {
		EXPECT_TRUE(matrix_matmult_ivec21[i] == result_matmult_ivec21[i]);
	}

	// multiply float matrices
	Matrix<float_type> matrix_matmult_fvec12 =
		matMultBasic(fMatrix1, fMatrix2);	 // 2x3 * 3x2
	std::vector<float_type> result_matmult_fvec12{68.098, 75.175, 160.564,
																								177.937};	 // 2x2

	EXPECT_TRUE(matrix_matmult_fvec12.size() ==
							result_matmult_fvec12.size());	// == 4
	for (size_t i = 0; i < matrix_matmult_fvec12.size(); ++i) {
		EXPECT_TRUE(matrix_matmult_fvec12[i] == result_matmult_fvec12[i]);
	}

	// multiply a float and a int matrix; result should be a float according to std::common_type<int,float>
	Matrix<float_type> matrix_matmult_fivec =
		matMultBasic(iMatrix1, fMatrix2);	 // 4x3 * 3x2
	std::vector<float_type> result_matmult_fivec{
		59.36, 65.51, 143.42, 158.93, 227.48, 252.35, 311.54, 345.77};	// 4x2
	EXPECT_TRUE(matrix_matmult_fivec.size() ==
							result_matmult_fivec.size());	 // == 8
	// todo: tighter bounds on deterministic results, float error propagation
	for (size_t i = 0; i < matrix_matmult_fivec.size(); ++i) {
		EXPECT_TRUE(
			(std::fabs(matrix_matmult_fivec[i] - result_matmult_fivec[i]) <= 1e-8));
	}
}

TEST_F(MatrixTest, MatrixMultiplicationTransposed) {

	// use the transposed matrix multiplication to improve cache hits
	auto matrix_matmult_fivec = MatMultTxp(iMatrix1, fMatrix2);	 // 4x3 * 3x2
	std::vector<float_type> result_matmult_fivec{
		59.36, 65.51, 143.42, 158.93, 227.48, 252.35, 311.54, 345.77};	// 4x2

	EXPECT_TRUE(matrix_matmult_fivec.size() ==
							result_matmult_fivec.size());	 // == 8

	for (size_t i = 0; i < matrix_matmult_fivec.size(); ++i) {
		EXPECT_TRUE(
			(std::fabs(matrix_matmult_fivec[i] - result_matmult_fivec[i]) <= 1e-8));
	}
}

TEST_F(MatrixTest, MatrixMultiplicationStrassen) {

	constexpr size_t n_row = 64;
	constexpr size_t n_col = 64;
	constexpr size_t size_matrix = n_row * n_col;

	// Test: multiply matrix with identity  
	std::vector<float_type> identity_matrix_vector(size_matrix, 0);
	// nrows = 64, ncols = stride = 64
	for (size_t i = 0, j = 0; i < n_row; i++) {
		identity_matrix_vector[n_col * i + i] = 1;
	}
	Matrix<float_type> identity_matrix(identity_matrix_vector, n_row, n_col);

	auto rndm_vec = GetUniformRandomNumbers<size_matrix>(0, 1, 42);
	Matrix<float_type> rndm_matrixA(rndm_vec, n_row, n_col);

	auto result_AI = MatMultStrassen(rndm_matrixA, identity_matrix);

	// Check : A x I = A 
	for (size_t i = 0; i < result_AI.size(); ++i) {
		EXPECT_TRUE(std::fabs(result_AI[i] - rndm_vec[i]) <= 1e-8);
	}

	// Test: multiply matrix with Diagonal matrix (scaling)
	float scalar = 5.49;
	auto diagonal_matrix = scalar*identity_matrix;

	auto result_diagonal_scalar = MatMultStrassen(diagonal_matrix, rndm_matrixA);
	// check : C = D x A  => C[ij] = d*A[ij]
	for (size_t i = 0; i < result_diagonal_scalar.size(); ++i) {
		EXPECT_TRUE(std::fabs(result_diagonal_scalar[i] - scalar*rndm_vec[i]) <= 1e-8);
	}

	// Test: constant matrix multiplication
	float f_constant1 = 3.0;
	std::vector<float_type> const1_matrix_vector(size_matrix, f_constant1);
	Matrix<float_type> const1_matrix(const1_matrix_vector, n_row, n_col);

	float f_constant2 = 4.5;
	std::vector<float_type> const2_matrix_vector(size_matrix, f_constant2);
	Matrix<float_type> const2_matrix(const2_matrix_vector, n_row, n_col);

	auto result_c1c2 =  MatMultStrassen(const2_matrix, const1_matrix);
	// check : C = A x B => C[ij] = nrow * (f_constant1 * f_constant2)
	for (size_t i = 0; i < result_diagonal_scalar.size(); ++i) {
		EXPECT_TRUE(std::fabs(result_c1c2[i] - (n_row*(f_constant1*f_constant2))) <= 1e-8);
	}



}