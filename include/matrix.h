#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <cassert>
#include <print>
#include <span>
#include <type_traits>
#include <vector>

/** Concepts */
/** 
* shouldnt work,
* would check if vector<T> has size that can be a mult  of two size_t objects.
* template <typename T, size_t A, size_t B>
* concept init_able_vector = requires(std::vector<T> vector, size_t a, size_t b) {
* 	requires vector.size() == a* b;
* };
*/

// todo: use concepts instead of SFINAE in the functions below.

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
	constexpr Matrix(data_type&& in_data, size_t&& nX, size_t&& nY) noexcept
			: m_{std::move(nX)},
				n_{std::move(nY)},
				size_(m_ * n_),
				elements_(std::move(in_data)) {}

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
	template <typename U, std::enable_if_t<std::is_floating_point_v<U>, int> = 0>
	explicit operator Matrix<U>() const {
		std::println("converting float to int");
		return Matrix<U>(std::vector<U>(elements_.begin(), elements_.end()), m_,
										 n_);
	}

	// no narrowing conversions
	template <typename U, std::enable_if_t<!std::is_floating_point_v<U>, int> = 0>
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
		for (unsigned int i = 0; i < size_; ++i) {
			elements_[i] += otherMatrix[i];	 // eqv to: otherMatrix.elements(i);
		}
		return *this;
	}

	// add assignment += generic
	template <typename U>
	Matrix& operator+=(const Matrix<U>& otherMatrix) noexcept {
		assert(size_ == otherMatrix.size());
		for (unsigned int i = 0; i < size_; ++i) {
			elements_[i] += static_cast<T>(otherMatrix[i]);
		}
		return *this;
	}

	// subtract assignment -=
	Matrix& operator-=(const Matrix& otherMatrix) noexcept {
		for (unsigned int i = 0; i < size_; ++i) {
			elements_[i] -= otherMatrix[i];
		}
		return *this;
	}

	// subtract assignment -= generic
	template <typename U>
	Matrix& operator-=(const Matrix<U>& otherMatrix) noexcept {
		assert(size_ == otherMatrix.size());
		for (unsigned int i = 0; i < size_; ++i) {
			elements_[i] -= static_cast<T>(otherMatrix[i]);
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

	// legal since c++23. earlier, operator could only take single subscipt.
	const T& operator[](const size_t i, const size_t j) const noexcept {
		return elements_[n_ * i + j];
	}

	T& operator[](const size_t i, const size_t j) noexcept {
		return elements_[n_ * i + j];
	}

	// property accessor
	const size_t size() const noexcept { return size_; }

	const auto shape() const noexcept { return std::make_pair(m_, n_); }

	const data_type& elements() const noexcept { return elements_; }

	const data_type& elements(size_t i) const noexcept { return elements_[i]; }

	/**
	*  friend functions 
	*/
	friend struct std::formatter<Matrix>;

	template <typename U, typename V>
	friend auto operator+(const Matrix<U>& lhs, const Matrix<V>& rhs);
	template <typename U, typename V>
	friend auto operator-(const Matrix<U>& lhs, const Matrix<V>& rhs);

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
/**
 * @brief check if matrix of different value_type are equal (MatrixA<U> == MatrixB)  
 * 
 * only valid for convertible types <U,V> or <V,U> : no narrowing type conversion of matrix is allowed.
 * 
 * @tparam U 
 * @tparam V 
 * @param MatrixA 
 * @param MatrixB 
 * @return true 
 * @return false 
 */
template <typename U, typename V,
					std::enable_if_t<std::is_convertible_v<U, V>, int> = 0>
bool operator==(const Matrix<U>& MatrixA, const Matrix<V>& MatrixB) {
	return ((MatrixA.elements() == MatrixB.elements()) &&
					(MatrixA.shape().first == MatrixB.shape().first) &&
					(MatrixA.shape().second == MatrixB.shape().second));
}

//!!!todo: why?? access to private data members. current access via function call. stupid imo.
//!!!todo: why?? Binaries can be declared as friends but may also just be regular functions.
/**
* Binary operator Overloads
*/

/**
* @brief Binary Arithmetic addition operator Overload
*/ //!!! use SFINAE HERE AS WELL, to cast from integral to float but not otherwise.
template <typename U, typename V>
auto operator+(const Matrix<U>& lhs, const Matrix<V>& rhs) {
	assert(lhs.size_ == rhs.size_);
	Matrix<typename std::common_type<U, V>::type> result(lhs);
	return result += rhs;	 // todo: std::move(result+=rhs);
												 // NRVO here or not???
}

/**
* @brief Binary Arithmetic Subtraction Overload
*/ //!!! use SFINAE HERE AS WELL, to cast from integral to float but not otherwise.
template <typename U, typename V>
auto operator-(const Matrix<U>& lhs, const Matrix<V>& rhs) {
	assert(lhs.size_ == rhs.size_);
	Matrix<typename std::common_type<U, V>::type> result(lhs);
	return result -= rhs;	 // todo: std::move(result+=rhs);
												 // NRVO here or not???
}

/**
* std::formatter overloads : easy printing 
*/

/**
 * @brief overload std::formatter for using std::print and std::println
 * prints matrix in with its given shape.
 * todo: use SFINAE to differ bw integral and floating point  
 */
template <typename U>
struct std::formatter<Matrix<U>> {
	constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

	auto format(const Matrix<U>& matrix, std::format_context& ctx) const {
		auto out = ctx.out();
		if (matrix.elements().empty()) {
			return std::format_to(out, "\n");
		} else {
			const auto [m_rows, m_columns] = matrix.shape();
			// auto m_rows = matrix.shape().first;
			// auto m_columns = matrix.shape().second;
			for (size_t i = 0; i < m_rows; ++i) {
				for (size_t j = 0; j < m_columns; ++j) {
					std::format_to(out, "{:0.6}\t", matrix[m_columns * i + j]);
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
 *  template specialisation for Matrix<int> 
 *  todo: use SFINAE to make this work with all std::integral
*/
template <>
struct std::formatter<Matrix<int>> {
	constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

	auto format(const Matrix<int>& matrix, std::format_context& ctx) const {
		auto out = ctx.out();
		if (matrix.elements().empty()) {
			return std::format_to(out, "\n");
		} else {
			const auto [m_rows, m_columns] = matrix.shape();
			// auto m_rows = matrix.shape().first;
			// auto m_columns = matrix.shape().second;
			for (size_t i = 0; i < m_rows; ++i) {
				for (size_t j = 0; j < m_columns; ++j) {
					std::format_to(out, "{}\t", matrix[m_columns * i + j]);
				}
				std::format_to(out, "\n");
			}
			return out;
		}
	}
};

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

	size_t M = input_matrix.shape().first;	 // rows of og matrix
	size_t N = input_matrix.shape().second;	 // columns of og matrix
	std::vector<T> txp_vector(input_matrix.size(), 0);

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
* @brief Multiplies two compatible matrices, A=MxP & B=PxN using objects of Matrix class. \
* @brief assert(A.P == B.P);
* @param Matrix<T> and Matrix<U> 
* @return Matrix<typename std::common_type<U, V>::type> C of shape M*N
* 
*/
template <typename U, typename V>
auto matMultBasic(const Matrix<U>& A, const Matrix<V>& B) {

	// the resultant matrix should be converted to appropriate type
	using common_t = typename std::common_type<U, V>::type;

	// const auto [a_rows,a_columns] = A.shape();
	// const auto [b_rows,b_columns] = B.shape();
	const size_t m = A.shape().first;
	const size_t p = A.shape().second;
	// A.n_ == B.m_ --> common index to sum over for the innermost loop
	const size_t n = B.shape().second;

	/** // todo:
	* !!! do you need to make this object here. 
	* !!! can simply construct from vector this while returning 
	* !!! store result in an vector/array 
	* !!! and then construct eg. : return Matrix<common_t>(result,m_, n_);
	*/
	Matrix<common_t> result(m, n);

	// todo: combine the first two loops. (i = 0; i<m*n; ++i)
	for (size_t i = 0; i < m; ++i) {
		for (size_t j = 0; j < n; ++j) {
			size_t res_idx = n * i + j;
			// float_type temp_sum  = 0.0F;
			for (size_t k = 0; k < p; ++k) {
				size_t a_idx = p * i + k;
				size_t b_idx = n * k + j;
				result[res_idx] += A[a_idx] * B[b_idx];
				// todo: repeated memory access to result[idx].
				// todo: instead store in temp and update value at the end of the loop.
				// temp_sum += A[a_idx] * B[b_idx];
			}
			// result[res_idx] += temp_sum;
		}
	}
	return result;
}

/** 
* @brief Multiple A=MxP & B=PxN by first transposing B to improve memory access patter
* @param 
* @return Matrix<typename std::common_type<U, V>::type>  C=M*N
*/
// assert(A.P == B.P);
template <typename U, typename V>
auto MatMultTxp(const Matrix<U>& A, const Matrix<V>& B) {
	using common_t = typename std::common_type<U, V>::type;

	// const auto [a_rows,a_columns] = A.shape();
	// const auto [b_rows,b_columns] = B.shape();
	const size_t m = A.shape().first;
	const size_t p = A.shape().second;
	const size_t n = B.shape().second;

	auto B_T = MatrixTransposeNaive(B);
	assert(p == B_T.shape().second);

	Matrix<common_t> result(m, n);

	// todo: combine the first two loops. (i = 0; i<m*n; ++i) { }
	for (size_t i = 0; i < m; ++i) {
		for (size_t j = 0; j < n; ++j) {
			size_t res_idx = n * i + j;
			size_t a_idx = (p * i);
			size_t b_idx = (p * j);	 // B_T.shape().second* j;
			for (size_t k = 0; k < p; ++k) {
				result[res_idx] += A[a_idx] * B_T[b_idx];
				a_idx += 1;	 // (p * i) + k;
				b_idx += 1;	 // (p * j) + k;
										 // todo: repeated memory access to result[idx].
				// todo: instead store in temp and update value at the end of the loop.
			}
		}
	}

	return result;
}

// !!! this is not really cstyle is it if its not using pointers.
// todo: make this with std::unique_pointer instead of spans.
/** 
*  @brief C = A*B with A=MxP & B=PxN in C-style without overhead of Classes etc.
* takes an array or vector or pointer to contiguous memory here, 
* and uses std::span as a non-owning reference to multiple two matrices.
* additionally takes the shape of the incoming matrices as a std::pair each.
* assert(A.P == B.P);
* @return Matrix<typename std::common_type<U, V>::type> C of shape M*N
*/
template <typename U, typename V>
auto cStyle_matMultBasic(std::span<const U> A, std::pair<size_t, size_t> shapeA,
												 std::span<const V> B,
												 std::pair<size_t, size_t> shapeB) {

	// the resultant matrix should be converted to appropriate type
	using common_t = typename std::common_type<U, V>::type;

	const size_t m = shapeA.first;
	const size_t p = shapeA.second;
	// shapeA.second == shapeB.first --> common index to sum over for the innermost loop
	const size_t n = shapeB.second;

	std::vector<common_t> result(m * n, 0.);

	for (size_t i = 0; i < m; ++i) {
		for (size_t j = 0; j < n; ++j) {
			size_t res_idx = n * i + j;
			for (size_t k = 0; k < p; ++k) {
				size_t a_idx = p * i + k;
				size_t b_idx = n * k + j;

				result[res_idx] += A[a_idx] * B[b_idx];
			}
		}
	}

	return result;
}

#endif
