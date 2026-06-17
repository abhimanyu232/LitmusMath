#ifndef MATRIX_OMP_H
#define MATRIX_OMP_H

#include <array>
#include <cassert>
#include <optional>
#include <print>
#include <span>
#include <type_traits>
#include <vector>

// #include <omp>

//!!! todo: use a policy based approach here, no code duplication.
//!!! or merge this with serial matrix since omp code can be easily serialized.

// todo: use concepts instead of SFINAE in the functions below.
namespace matrix_omp {

/**
* @brief Matrix class using std::vector
* can be implemented, probably faster using compile time sizes and std::array.
* the size_ (or shape) will then have to be given as template parameters.
*/
template <typename T>
class Matrix {

	using data_type = std::vector<T>;
	using value_type = typename data_type::value_type;

 public:
	/**
 	* constructors
 	*/

	// !!! Not Safe : none of the ctors are safe since (in_data.size() == nX*nY) IS NOT GUARANTEED
	// !!! HOW TO ENSURE WITHOUT USING STATIC ASSERTS???

	// default init; empty matrix, size=0, but 16*sizeof(T) memory reserved
	constexpr Matrix() noexcept : m_{0}, n_{0}, size_(0), elements_(0) {
		elements_.reserve(16);
	}

	// zero intialize with given shape (and hence size)
	constexpr Matrix(const size_t nX, const size_t nY) noexcept
			: m_{nX}, n_{nY}, size_(nX * nY), elements_(data_type(size_, 0)) {}

	// intialize with given vector and shape
	constexpr Matrix(const data_type& in_data, const size_t nX,
									 const size_t nY) noexcept
			: m_{nX}, n_{nY}, size_(m_ * n_), elements_(in_data) {}

	// move given vector and shape // todo: should everything be r-val ref here??
	constexpr Matrix(data_type&& in_data, size_t nX, size_t nY) noexcept
			: m_{nX}, n_{nY}, size_(m_ * n_), elements_(std::move(in_data)) {}

	// copy constructors
	constexpr Matrix(const Matrix& otherMatrix) noexcept = default;
	// move constructors
	constexpr Matrix(Matrix&& in_matrix) noexcept = default;

	// copy assignment
	Matrix& operator=(const Matrix& lhs) noexcept = default;

	// todo: move assignment
	// Matrix& operator=(const Matrix&& lhs) noexcept = default;
	// template <typename U>
	// const Matrix& operator=(const Matrix<U>& otherMatrix) const noexcept {
	// 	return Matrix<T>(otherMatrix);
	// }

 public:
	/**   
	* operator overloads
	*/

	// cast allowed only for integral-> floating point type
	// template <typename U, std::enable_if_t<std::is_floating_point_v<U>, int> = 0>
	template <std::floating_point U>
	explicit operator Matrix<U>() const {
		std::println("converting int to float");
		return Matrix<U>(std::vector<U>(elements_.begin(), elements_.end()), m_,
										 n_);
	}

	// no narrowing conversions
	// template <typename U, std::enable_if_t<!std::is_floating_point_v<U>, int> = 0>
	template <typename U>
		requires(!std::floating_point<U>)
	operator Matrix<U>() const = delete;

	// spaceship comparison operator // also default generates operator==
	auto operator<=>(const Matrix& rhs) const = default;

	// todo: should specialize operator==() separately for integral types : can be equal
	// todo: and floating point types : check using a delta i.e.  operator== implies (a-b)<epsilon

	// not the correct implementation, will check Matrix<T> == Matrix<U> which should be illegal
	// template<typename U, std::enable_if_t< std::is_integral_v<U>,int> = 0>
	// auto operator==(const Matrix<U>& rhs) const {}

	// template<typename U, std::enable_if_t< std::is_floating_point_v<U>,int> = 0>
	// auto operator==(const Matrix<U>& rhs) const {}

	/**  
	 * unary airthmetic operators
	*/

	// add assignment +=
	Matrix& operator+=(const Matrix& otherMatrix) noexcept {
		assert(size_ == otherMatrix.size());
		for (size_t i = 0; i < size_; ++i) {
			elements_[i] += otherMatrix[i];	 // eqv to: otherMatrix.elements(i);
		}
		return *this;
	}

	// add assignment += generic
	//!!! silent conversion here from U -> T
	template <typename U>
	Matrix& operator+=(const Matrix<U>& otherMatrix) noexcept {
		assert(size_ == otherMatrix.size());
		for (size_t i = 0; i < size_; ++i) {
			elements_[i] += static_cast<T>(otherMatrix[i]);
		}
		return *this;
	}

	// subtract assignment -=
	Matrix& operator-=(const Matrix& otherMatrix) noexcept {
		assert(size_ == otherMatrix.size());
		for (size_t i = 0; i < size_; ++i) {
			elements_[i] -= otherMatrix[i];
		}
		return *this;
	}

	// subtract assignment -= generic
	//!!! silent conversion here from U -> T
	template <typename U>
	Matrix& operator-=(const Matrix<U>& otherMatrix) noexcept {
		assert(size_ == otherMatrix.size());
		for (size_t i = 0; i < size_; ++i) {
			elements_[i] -= static_cast<T>(otherMatrix[i]);
		}
		return *this;
	}

	// matrix-scalar operations

	// scalar multiply assignment *=
	Matrix& operator*=(T scalar) noexcept {
		for (size_t i = 0; i < size_; ++i) {
			elements_[i] *= scalar;
		}
		return *this;
	}

	// scalar multiply assignment *= generic
	template <typename U>
	Matrix& operator*=(U scalar) noexcept {
		for (size_t i = 0; i < size_; ++i) {
			elements_[i] *= static_cast<T>(scalar);
		}
		return *this;
	}

	// scalar divide assignement /=
	Matrix& operator/=(T scalar) noexcept {
		for (size_t i = 0; i < size_; ++i) {
			elements_[i] /= scalar;
		}
		return *this;
	}

	// scalar divide assignment /= generic
	template <typename U>
	Matrix& operator/=(U scalar) noexcept {
		for (size_t i = 0; i < size_; ++i) {
			elements_[i] /= static_cast<T>(scalar);
		}
		return *this;
	}

	// 1D index operator
	const T& operator()(const size_t i) const noexcept { return elements_[i]; }

	T& operator()(const size_t i) noexcept { return elements_[i]; }

	const T& operator[](const size_t i) const noexcept { return elements_[i]; }

	T& operator[](const size_t i) noexcept { return elements_[i]; }

	// 2D index operator
	const T& operator()(const size_t i, const size_t j) const noexcept {
		return elements_[n_ * i + j];
	}

	T& operator()(const size_t i, const size_t j) noexcept {
		return elements_[n_ * i + j];
	}

	// only legal since c++23. earlier, operator[] could only take single subscipt.
	const T& operator[](const size_t i, const size_t j) const noexcept {
		return elements_[n_ * i + j];
	}

	T& operator[](const size_t i, const size_t j) noexcept {
		return elements_[n_ * i + j];
	}

	// property accessor
	[[nodiscard]] const size_t size() const noexcept { return size_; }

	[[nodiscard]] const auto shape() const noexcept {
		return std::make_pair(m_, n_);
	}

	[[nodiscard]] const data_type& elements() const noexcept { return elements_; }

	[[nodiscard]] const data_type& elements(size_t i) const noexcept {
		return elements_[i];
	}

	/**
	*  friend functions 
	*/
	friend struct std::formatter<Matrix>;

	// template <typename U, typename V>
	// friend auto operator+(const Matrix<U>& lhs, const Matrix<V>& rhs);
	// template <typename U, typename V>
	// friend auto operator-(const Matrix<U>& lhs, const Matrix<V>& rhs);

 protected:
	// protected members

 private:
	// private members
	size_t m_;
	size_t n_;
	size_t size_;
	data_type elements_;
};

// Comparison Operators
// todo: should specialize operator==() separately for integral types : can be equal
// todo: and floating point types : check using a delta i.e.  operator== implies (a-b)<epsilon
// only well defined for matrices of the same or similar type

// compiler does arguement switching to find overload match,
// this should therefore imply std::is_convertible_v<V,U> if !(std::is_convertible_v<U,V>)
// hence this should be good for a commutative check.
// /**
//  * @brief check if matrix of different value_type are equal (MatrixA<U> == MatrixB)  
//  * 
//  * only valid for convertible types <U,V> or <V,U> : no narrowing type conversion of matrix is allowed.
//  * 
//  * @tparam U 
//  * @tparam V 
//  * @param MatrixA 
//  * @param MatrixB 
//  * @return true 
//  * @return false 
//  */
// template <typename U, typename V,
// 					std::enable_if_t<std::is_convertible_v<U, V>, int> = 0>
// bool operator==(const Matrix<U>& MatrixA, const Matrix<V>& MatrixB) {
// 	return ((MatrixA.elements() == MatrixB.elements()) &&
// 					(MatrixA.shape().first == MatrixB.shape().first) &&
// 					(MatrixA.shape().second == MatrixB.shape().second));
// }

//!!!todo: why?? access to private data members. current access via function call. stupid imo.
//!!!todo: why?? Binaries can be declared as friends but may also just be regular functions.
/**
* Binary operator Overloads
*/

/**
* @brief Binary Arithmetic addition operator Overload
*/ //!!! use SFINAE HERE AS WELL, to cast from integral to float but not otherwise.
template <typename U, typename V>
	requires (std::common_with<U, V>)
auto operator+(const Matrix<U>& lhs, const Matrix<V>& rhs) {
	assert(lhs.size() == rhs.size());
	Matrix<typename std::common_type<U, V>::type> result(lhs);
	return result += rhs;	 // todo: std::move(result+=rhs);
												 // NRVO here or not???
}

/**
* @brief Binary Arithmetic Subtraction Overload
*/ //!!! use SFINAE HERE AS WELL, to cast from integral to float but not otherwise.
template <typename U, typename V>
	requires (std::common_with<U, V>)
auto operator-(const Matrix<U>& lhs, const Matrix<V>& rhs) {
	assert(lhs.size() == rhs.size());
	Matrix<typename std::common_type<U, V>::type> result(lhs);
	return result -= rhs;	 // todo: std::move(result+=rhs);
												 // NRVO here or not???
}

/**
* @brief Binary Matrix Scalar Multiply Overload
*/ //!!! use SFINAE HERE AS WELL, to cast from integral to float but not otherwise.
template <typename U, typename V>
	requires (std::common_with<U, V>)
auto operator*(const Matrix<U>& lhs_matrix, const V rhs_scalar) noexcept {
	Matrix<typename std::common_type<U, V>::type> result(lhs_matrix);

	return result *=
				 static_cast<typename std::common_type<U, V>::type>(rhs_scalar);
}

template <typename U, typename V>
auto operator*(const V lhs_scalar, const Matrix<U>& rhs_matrix) noexcept {
	return rhs_matrix * lhs_scalar;
}

/**
* @brief Binary Matrix Scalar Division Overload
*/ //!!! use SFINAE HERE AS WELL, to cast from integral to float but not otherwise.
template <typename U, typename V>
	requires (std::common_with<U, V>)
auto operator/(const Matrix<U>& lhs_matrix, const V rhs_scalar) noexcept {
	Matrix<typename std::common_type<U, V>::type> result(lhs_matrix);

	return result /=
				 static_cast<typename std::common_type<U, V>::type>(rhs_scalar);
}

template <typename U, typename V>
auto operator/(const V lhs_scalar, const Matrix<U>& rhs_matrix) noexcept {
	return rhs_matrix / lhs_scalar;
}

/**
* Matrix Manipulation Functions
*/

/** 
*  @brief returns a Matrix object containing the transpose of input.
*  @param Matrix<T> A of shape MxN 
*  @return Matrix<T> A_T of shape NxM such that A_T[i][j] = A[j][i] 
*/
template <typename T>
auto MatrixTransposeNaive(const Matrix<T>& input_matrix) {

	const size_t M = input_matrix.shape().first;	 // rows of og matrix
	const size_t N = input_matrix.shape().second;	 // columns of og matrix
	std::vector<T> txp_vector(input_matrix.size(), 0);

//!!! OMP todo: check memory access, cache thrashing/repeated writes.
//!!! OMP<=2.0 REQUIRES signed loop indices. unsigned int => UB
//!!! OMP>3.0 is okay with unsigned loop
#pragma omp parallel for
	for (size_t k = 0; k < M * N; ++k) {
		size_t i = k / M;
		size_t j = k % M;
		txp_vector[k] = input_matrix[(N * j) + i];
	}

	return Matrix<T>(txp_vector, N, M);
}

/**
* Matrix Multiplication Functions
*/

/** 
* @brief Multiplies two compatible matrices, matrixA=MxP & matrixB=PxN using objects of Matrix class.
* @brief assert(matrixA.P == matrixB.P);
* @param Matrix<T> and Matrix<U> 
* @return Matrix<typename std::common_type<U, V>::type> C of shape M*N
* 
*/
template <typename U, typename V>
	requires (std::common_with<U, V>)
auto matMultBasic(const Matrix<U>& matrixA, const Matrix<V>& matrixB) {

	// the resultant matrix should be converted to appropriate type
	using common_t = typename std::common_type<U, V>::type;

	const size_t M = matrixA.shape().first;
	const size_t P = matrixA.shape().second;
	// matrixA.n_ == matrixB.m_ --> common index to sum over for the innermost loop
	const size_t N = matrixB.shape().second;

	/** // todo:
	* !!! do you need to make this object here. 
	* !!! can simply construct from vector this while returning 
	* !!! store result in an vector/array 
	* !!! and then construct eg. : return Matrix<common_t>(result,m_, n_);
	*/
	Matrix<common_t> result(M, N);

///// todo: combine the first two loops. (i = 0; i<M*N; ++i)
//!!! openMP todo: check memory access, cache thrashing/repeated writes.
#pragma omp parallel for collapse(2)
	for (size_t i = 0; i < M; ++i) {
		for (size_t j = 0; j < N; ++j) {
			size_t res_idx = N * i + j;
			// float_type temp_sum  = 0.0;
			for (size_t k = 0; k < P; ++k) {
				size_t a_idx = P * i + k;
				size_t b_idx = N * k + j;
				result[res_idx] += matrixA[a_idx] * matrixB[b_idx];
				// todo: repeated memory access to result[idx].
				// todo: instead store in temp and update value at the end of the loop.
				// temp_sum += matrixA[a_idx] * matrixB[b_idx];
			}
			// result[res_idx] += temp_sum;
		}
	}
	return result;
}

/** 
* @brief Multiply matrixA=MxP & matrixB=PxN by first transposing matrixB to improve memory access patter
* @param 
* @return Matrix<typename std::common_type<U, V>::type>  C=M*N
*/
// assert(matrixA.P == matrixB.P);
template <typename U, typename V>
	requires (std::common_with<U, V>)
auto MatMultTxp(const Matrix<U>& matrixA, const Matrix<V>& matrixB) {
	using common_t = typename std::common_type<U, V>::type;

	// const auto [a_rows,a_columns] = matrixA.shape();
	// const auto [b_rows,b_columns] = matrixB.shape();
	const size_t M = matrixA.shape().first;
	const size_t P = matrixA.shape().second;
	const size_t N = matrixB.shape().second;

	auto matrixB_Txp = MatrixTransposeNaive(matrixB);
	assert(P == matrixB_Txp.shape().second);

	Matrix<common_t> result(M, N);

///// todo: combine the first two loops. (i = 0; i<M*N; ++i) { }
//!!! openMP todo: check memory access, cache thrashing/repeated writes.
#pragma omp parallel for collapse(2)
	for (size_t i = 0; i < M; ++i) {
		for (size_t j = 0; j < N; ++j) {
			size_t res_idx = N * i + j;
			size_t a_idx = (P * i);
			size_t b_idx = (P * j);	 // B_T.shape().second* j;
			for (size_t k = 0; k < P; ++k) {
				result[res_idx] += matrixA[a_idx] * matrixB_Txp[b_idx];
				a_idx += 1;	 // (P * i) + k;
				b_idx += 1;	 // (P * j) + k;

				// todo: repeated memory access to result[idx].
				// todo: instead store in temp and update value at the end of the loop.
			}
		}
	}

	return result;
}

//todo: Matrix class member function: generate a matrix "view" of a subrange of the matrix
/**
 * @brief Wrapper for calling the cache ambivalent Strassen algorithm for Matrix Multiplication 
 * 
 * @tparam U 
 * @tparam V 
 * @param matrixA 
 * @param matrixB 
 * @return auto 
 */
template <typename U, typename V>
	requires (std::common_with<U, V>)
auto MatMultStrassen(const Matrix<U>& matrixA, const Matrix<V>& matrixB,
										 const bool do_full_strassen = false) {
	using common_t = typename std::common_type<U, V>::type;
	const size_t M = matrixA.shape().first;
	const size_t P = matrixA.shape().second;
	const size_t Q = matrixB.shape().first;	 // assert (P==Q);
	const size_t N = matrixB.shape().second;

	assert(P == Q);

	// todo!!!: IMPLEMENT PadMatrix, MatMultStrassen_Impl
	// step1: pad the matrices if the size is not power of two.
	size_t size_to_pad = 0;

	auto matrixA_padded = PadMatrix(matrixA, size_to_pad);
	auto matrixB_padded = PadMatrix(matrixB, size_to_pad);
	// step2: pass to strassen recursive wrapper
	auto result_padded =
		MatMultStrassen_Impl(matrixA_padded, matrixB_padded, do_full_strassen);

	auto result = result_padded;
	// Matrix<common_t> result(M, N);
	return result;
}

/**
 * @brief Helper function to pad matrix to the nearest power of 2 for use with Strassen Algorithm.
 * !!!todo: implement padding if needed. 
 * @param Matrix<U> inputMatrix 
 * @param size_t paddingSize 
 * @return Matrix<U> 
 */
template <typename U>
Matrix<U> PadMatrix(Matrix<U> inputMatrix, const size_t paddingSize = 0) {
	if (paddingSize == 0)
		return inputMatrix;
	else {
		std::println("padding method not implemented");
		std::exit(1);
	}
}

/// @brief Implementation of Strassens Algorithm for Matrix Multiplication
///
/// For small matrices, option of delegating to standard matmul or
/// continue with Strassen's till the matrix is too small to subdivide (not recommended)
/// @param Matrix<U> matrixA
/// @param Matrix<V> matrixB
/// @return Matrix<common_type<U, V>> res_matrix
template <typename U, typename V>
	requires (std::common_with<U, V>)
Matrix<typename std::common_type<U, V>::type> MatMultStrassen_Impl(
	const Matrix<U>& matrixA, const Matrix<V>& matrixB,
	bool do_full_strassen = false) {

	const size_t threshold = 1024;	// 32x32, 2^10, n = 10

	// for a small enough matrix, do standard matrix multiplication
	if (!do_full_strassen && (matrixA.size() <= threshold)) [[unlikely]] {

		return matMultBasic(matrixA, matrixB);

		// handle a 2x2 matrix using Strassens, lowest size square matrix
	} else if (do_full_strassen && (matrixA.size() == 4)) [[unlikely]] {

		// no need to call getQuad, elements accessible directly.
		auto quadMatrixVectorA = matrixA.elements();	// copy data
		auto quadMatrixVectorB = matrixB.elements();

		auto M1 = (quadMatrixVectorA[0] + quadMatrixVectorA[3]) *
							(quadMatrixVectorB[0] + quadMatrixVectorB[3]);

		auto M2 =
			(quadMatrixVectorA[2] + quadMatrixVectorA[3]) * (quadMatrixVectorB[0]);

		auto M3 =
			(quadMatrixVectorA[0]) * (quadMatrixVectorB[1] - quadMatrixVectorB[3]);

		auto M4 =
			(quadMatrixVectorA[3]) * (quadMatrixVectorB[2] - quadMatrixVectorB[0]);

		auto M5 =
			(quadMatrixVectorA[0] + quadMatrixVectorA[1]) * (quadMatrixVectorB[3]);

		auto M6 = (quadMatrixVectorA[2] - quadMatrixVectorA[0]) *
							(quadMatrixVectorB[0] + quadMatrixVectorB[1]);

		auto M7 = (quadMatrixVectorA[1] - quadMatrixVectorA[3]) *
							(quadMatrixVectorB[2] + quadMatrixVectorB[3]);

		auto C11 = M1 + M4 - M5 + M7;
		auto C12 = M3 + M5;
		auto C21 = M2 + M4;
		auto C22 = (M1 - M2) + (M3 + M6);

		std::vector<typename std::common_type<U, V>::type> assembledVector{
			C11, C12, C21, C22};

		return Matrix<typename std::common_type<U, V>::type>(assembledVector, 2, 2);

	} else	// continue with Strassen's algorithm, and partition the matrix further
	{
		// split the matrix into quads.
		// A11 = quadMatrixVectorA[0], A12 = quadMatrixVectorA[1], A21 = quadMatrixVectorA[2], A22 = quadMatrixVectorA[3]
		auto quadMatrixVectorA = getQuadMatrices(matrixA);

		// B11 = quadMatrixVectorB[0], B12 = quadMatrixVectorB[1], B21 = quadMatrixVectorB[2],  B22 = quadMatrixVectorB[3]
		auto quadMatrixVectorB = getQuadMatrices(matrixB);

		/* recursive calls
				Matrix_t M1 = MatMultStrassen_Impl(A11 + A22, B11 + B22);
        Matrix_t M2 = MatMultStrassen_Impl(A21 + A22, B11);
        Matrix_t M3 = MatMultStrassen_Impl(A11, B12 - B22);
        Matrix_t M4 = MatMultStrassen_Impl(A22, B21 - B11);
        Matrix_t M5 = MatMultStrassen_Impl(A11 + A12, B22);
        Matrix_t M6 = MatMultStrassen_Impl(A21 - A11, B11 + B12);
        Matrix_t M7 = MatMultStrassen_Impl(A12 - A22, B21 + B22);
		*/
		auto M1 = MatMultStrassen_Impl(quadMatrixVectorA[0] + quadMatrixVectorA[3],
																	 quadMatrixVectorB[0] + quadMatrixVectorB[3]);
		auto M2 = MatMultStrassen_Impl(quadMatrixVectorA[2] + quadMatrixVectorA[3],
																	 quadMatrixVectorB[0]);
		auto M3 = MatMultStrassen_Impl(quadMatrixVectorA[0],
																	 quadMatrixVectorB[1] - quadMatrixVectorB[3]);
		auto M4 = MatMultStrassen_Impl(quadMatrixVectorA[3],
																	 quadMatrixVectorB[2] - quadMatrixVectorB[0]);
		auto M5 = MatMultStrassen_Impl(quadMatrixVectorA[0] + quadMatrixVectorA[1],
																	 quadMatrixVectorB[3]);
		auto M6 = MatMultStrassen_Impl(quadMatrixVectorA[2] - quadMatrixVectorA[0],
																	 quadMatrixVectorB[0] + quadMatrixVectorB[1]);
		auto M7 = MatMultStrassen_Impl(quadMatrixVectorA[1] - quadMatrixVectorA[3],
																	 quadMatrixVectorB[2] + quadMatrixVectorB[3]);
		/*
        Matrix_t C11 = M1 + M4 - M5 + M7;
        Matrix_t C12 = M3 + M5;           
        Matrix_t C21 = M2 + M4;
        Matrix_t C22 = M1 - M2 + M3 + M6;
		*/
		auto C11 = (M1 + M4) - M5 + M7;
		auto C12 = M3 + M5;
		auto C21 = M2 + M4;
		auto C22 = (M1 - M2) + (M3 + M6);

		return assembleMatrixfromQuads(C11, C12, C21, C22);
	}

	// return std::nullopt;
}

//!!! optimize: copies full  matrix data, mem heavy,
//todo: should be a member function, so that you can do matrix.getQuads();
/**
 * @brief Subdivide the matrix into 4 quad matrices and return as a vector of matrix objects
 * 
 * @param Matrix<T> matrixA 
 * @return std::vector<Matrix<T>> quadMatrices
 */
template <typename T>
auto getQuadMatrices(const Matrix<T>& matrixA) {
	// todo: possible check to see if the matrix is able to be split i.e if matrixA.size() >= 4

	std::vector<T> matrix_data = matrixA.elements();	// copy data
	size_t matrix_size = matrixA.size();
	size_t half_matrix_size = matrix_size / 2;

	// shape(m,n) = rows x cols = (rows=ny,cols=nx)
	size_t matA_nrows = matrixA.shape().first;	 // num rows
	size_t matA_ncols = matrixA.shape().second;	 // num cols

	//!!! todo : prior check if shape is divisible by 2.
	size_t quad_mat_nrows = matA_nrows / 2;
	size_t quad_mat_ncols = matA_ncols / 2;

	std::vector<std::vector<T>> quad_vectors(4);
	// each vector has size quad_mat_ncols*quad_mat_nrows

	//!!! openMP todo: #pragma omp parallel for
	// todo: always loops from 0 to 4, to generate quad matrices,
	// todo: should be easily split into four threads
	// todo: probably have to refactor the code here.

	// note: alreads kind of vectorised loads if you think about it,
	// note: loading  2 "rows" at a time
	// load each row ; stride = number of cols.

	// #pragma omp parallel for num_threads(4)
	for (size_t j = 0, i = 0; i < matrix_size; i += matA_ncols) {

		//!!! why use floor here, both are integral types
		j = 2 * std::floor(i / half_matrix_size);

		quad_vectors[j].insert(quad_vectors[j].end(),
													 std::next(matrix_data.begin(), i),
													 std::next(matrix_data.begin(), i + quad_mat_ncols));

		quad_vectors[j + 1].insert(
			quad_vectors[j + 1].end(),
			std::next(matrix_data.begin(), i + quad_mat_ncols),
			std::next(matrix_data.begin(), i + matA_ncols));
	}

	// std::forward??
	return std::vector<Matrix<T>>{
		Matrix<T>(quad_vectors[0], quad_mat_nrows, quad_mat_ncols),
		Matrix<T>(quad_vectors[1], quad_mat_nrows, quad_mat_ncols),
		Matrix<T>(quad_vectors[2], quad_mat_nrows, quad_mat_ncols),
		Matrix<T>(quad_vectors[3], quad_mat_nrows, quad_mat_ncols)};
}

/**
 * @brief Assemble a full matrix from four quad-matrices. 
 * 
 * @tparam U 
 * @param matrix11 
 * @param matrix12 
 * @param matrix21 
 * @param matrix22 
 * @return const Matrix<U> assembled_matrix
 */
template <typename U>
Matrix<U> assembleMatrixfromQuads(const Matrix<U>& matrix11,
																	const Matrix<U>& matrix12,
																	const Matrix<U>& matrix21,
																	const Matrix<U>& matrix22) {

	//!!! todo : prior check if shape is divisible by 2.
	size_t quad_mat_nrows = matrix11.shape().first;
	size_t quad_mat_ncols = matrix11.shape().second;

	// shape(m,n) = rows x cols = (rows=ny,cols=nx)
	size_t assm_mat_nrows = quad_mat_nrows * 2;	 // num rows
	size_t assm_mat_ncols = quad_mat_ncols * 2;	 // num cols

	size_t assm_mat_size = assm_mat_nrows * assm_mat_ncols;
	size_t half_assm_mat_size = assm_mat_size / 2;

	std::vector<U>
		assembled_matrix_vector;	// un-init size. to init assembled_matrix_vector(assm_mat_size)
	assembled_matrix_vector.reserve(assm_mat_size);

	std::vector<std::vector<U>> quad_vectors(4);
	quad_vectors[0].insert(quad_vectors[0].end(), matrix11.elements().begin(),
												 matrix11.elements().end());
	quad_vectors[1].insert(quad_vectors[1].end(), matrix12.elements().begin(),
												 matrix12.elements().end());
	quad_vectors[2].insert(quad_vectors[2].end(), matrix21.elements().begin(),
												 matrix21.elements().end());
	quad_vectors[3].insert(quad_vectors[3].end(), matrix22.elements().begin(),
												 matrix22.elements().end());

	//!!! openMP todo: #pragma omp parallel for
	// todo: always loops from 0 to 4, to generate quad matrices,
	// todo: should be easily split into four threads
	// todo: probably have to refactor the code here.

	// i = 0,1,2....assm_nrows.
	// #pragma omp parallel for num_threads(4)
	for (size_t j = 0, k = 0, i = 0; i < assm_mat_nrows; i++) {

		//!!! why use floor here, both are integral types
		j = 2 * (std::floor(i / (quad_mat_nrows)));
		// i=0,1,2,3 -> j = 0 ; i=4,5,6,7 -> j = 2 if for example given that assm_mat_nrows = 8.
		k = (i % (quad_mat_nrows)) * quad_mat_ncols;

		assembled_matrix_vector.insert(
			assembled_matrix_vector.end(), std::next(quad_vectors[j].begin(), k),
			std::next(quad_vectors[j].begin(), k + quad_mat_ncols));

		assembled_matrix_vector.insert(
			assembled_matrix_vector.end(), std::next(quad_vectors[j + 1].begin(), k),
			std::next(quad_vectors[j + 1].begin(), k + quad_mat_ncols));
	}

	return Matrix<U>(assembled_matrix_vector, assm_mat_nrows, assm_mat_ncols);
}

// !!! this is not really cstyle is it if its not using pointers.
// todo: make this with std::unique_pointer instead of spans.
/** 
*  @brief C = matrixA*matrixB with matrixA=MxP & matrixB=PxN in C-style without overhead of Classes etc.
*
* takes an array or vector or pointer to contiguous memory here, 
* and uses std::span as a non-owning reference to multiple two matrices.
*
* additionally takes the shape of the incoming matrices as a std::pair each.
* assert(matrixA.P == matrixB.P);
* @return Matrix<typename std::common_type<U, V>::type> C of shape M*N
*/
template <typename U, typename V>
	requires (std::common_with<U, V>)
auto cStyle_matMultBasic(std::span<const U> matrixA,
												 std::pair<size_t, size_t> shapeA,
												 std::span<const V> matrixB,
												 std::pair<size_t, size_t> shapeB) {

	// the resultant matrix should be converted to appropriate type
	using common_t = typename std::common_type<U, V>::type;

	const size_t M = shapeA.first;
	const size_t P = shapeA.second;
	// shapeA.second == shapeB.first --> common index to sum over for the innermost loop
	const size_t N = shapeB.second;

	std::vector<common_t> result(M * N, 0.);

	//!!! openMP todo: #pragma omp parallel for
#pragma omp parallel for collapse(2)
	for (size_t i = 0; i < M; ++i) {
		for (size_t j = 0; j < N; ++j) {
			size_t res_idx = N * i + j;
			for (size_t k = 0; k < P; ++k) {
				size_t a_idx = P * i + k;
				size_t b_idx = N * k + j;

				result[res_idx] += matrixA[a_idx] * matrixB[b_idx];
			}
		}
	}

	return result;
}

}	 // namespace matrix_omp

/**
* std::formatter overloads : easy printing 
*/

/**
 * @brief overload std::formatter for using std::print and std::println
 * prints matrix in with its given shape.
 * 
 * todo: use SFINAE to differ bw integral and floating point  
 */
template <typename U>
struct std::formatter<matrix_omp::Matrix<U>> {
	constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

	auto format(const matrix_omp::Matrix<U>& matrix,
							std::format_context& ctx) const {
		auto out = ctx.out();
		if (matrix.elements().empty()) {
			return std::format_to(out, "\n");
		} else {
			const auto [m_rows, m_columns] = matrix.shape();
			for (size_t i = 0; i < m_rows; ++i) {
				for (size_t j = 0; j < m_columns; ++j) {
					if constexpr (std::integral<U>) {
						// special stream handling for Matrix<int> 
						std::format_to(out, "{}\t", matrix[m_columns * i + j]);
					} else {
						std::format_to(out, "{:0.6}\t", matrix[m_columns * i + j]);
					}
				}
				std::format_to(out, "\n");
			}
			return out;
		}
	}
};

/**
 * @brief overload std::formatter for using std::print and std::println
 * prints matrix in with its given shape.
 * 
 *  template specialisation for Matrix<int> 
 *  todo: use SFINAE to make this work with all std::integral
*/
// template <>
// struct std::formatter<matrix_omp::Matrix<int>> {
// 	constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

// 	auto format(const matrix_omp::Matrix<int>& matrix,
// 							std::format_context& ctx) const {
// 		auto out = ctx.out();
// 		if (matrix.elements().empty()) {
// 			return std::format_to(out, "\n");
// 		} else {
// 			const auto [m_rows, m_columns] = matrix.shape();
// 			// auto m_rows = matrix.shape().first;
// 			// auto m_columns = matrix.shape().second;
// 			for (size_t i = 0; i < m_rows; ++i) {
// 				for (size_t j = 0; j < m_columns; ++j) {
// 					std::format_to(out, "{}\t", matrix[m_columns * i + j]);
// 				}
// 				std::format_to(out, "\n");
// 			}
// 			return out;
// 		}
// 	}
// };

#endif
