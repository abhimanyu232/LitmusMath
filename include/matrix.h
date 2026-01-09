#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <print>
#include <span>
#include <vector>

// shouldnt work,
// would check if vector<T> has size that can be a mult  of two size_t objects.
template <typename T>
concept init_able_vector = requires(std::vector<T> vector, size_t a, size_t b) {
	requires vector.size() == a* b;
};

// Matrix class using std::vector
// can be implemented, probably faster using compile time sizes and std::array.
// the size_ (or shape) will then have to be given as template parameters.
template <typename T>
class Matrix {

	using data_type = std::vector<T>;
	using value_type = typename data_type::value_type;

 public:
	// constructors
	constexpr Matrix() noexcept : m_{0}, n_{0}, size_(0), elements_(0) {
		elements_.reserve(16);
	}

	// zero init with some given shape
	constexpr Matrix(const size_t nX, const size_t nY) noexcept
			: m_{nX}, n_{nY}, size_(nX * nY), elements_(data_type(size_, 0)) {}

	// !!! Not Safe : none of the ctors are safe since (in_data.size() == nX*nY) IS NOT GUARANTEED
	// !!! HOW TO ENSURE WITHOUT USING STATIC ASSERTS???

	// init with given vector and shape
	constexpr Matrix(data_type& in_data, size_t nX, size_t nY) noexcept
			: m_{nX}, n_{nY}, size_(m_ * n_), elements_(in_data) {}

	// using r-value with given vector and shape
	constexpr Matrix(data_type&& in_data, size_t nX, size_t nY) noexcept
			: m_{nX}, n_{nY}, size_(m_ * n_), elements_(std::move(in_data)) {}

	// copy constructors
	// copy from Matrix of same type
	// todo: should we care about copies vs move?
	constexpr Matrix(const Matrix& otherMatrix) noexcept
			: m_{otherMatrix.shape().first},
				n_{otherMatrix.shape().second},
				size_(m_ * n_),
				elements_(otherMatrix.elements()) {}

	// copy from a matrix of a different type.
	// type converts to Matrix<T> from Matrix<U>
	template <typename U>
	constexpr Matrix(const Matrix<U>& otherMatrix) noexcept
			: m_{otherMatrix.shape().first},
				n_{otherMatrix.shape().second},
				size_(m_ * n_),
				elements_(otherMatrix.elements().begin(),
									otherMatrix.elements().end()) {}

 public:
	//  overloads

	// copy from a vector
	const T& operator=(const data_type& t_data) const noexcept {
		assert(size_ == t_data.size());
		elements_ = t_data;

		return elements_;
	}

	// copy from a vector of different type
	template <typename U>
	const T& operator=(const std::span<U> u_data) const noexcept {
		assert(size_ == u_data.size());
		for (size_t i = 0; i < size_; ++i)
			elements_[i] = static_cast<T>(u_data[i]);

		return elements_;
	}

	// arithmetic add + operator overloads
	Matrix& operator+=(const Matrix& otherMatrix) noexcept {
		assert(size_ == otherMatrix.size());
		for (unsigned int i = 0; i < size_; ++i) {
			elements_[i] += otherMatrix[i];	 // eqv to: otherMatrix.elements(i);
		}
		return *this;
	}

	template <typename U>
	Matrix& operator+=(const Matrix<U>& otherMatrix) noexcept {
		assert(size_ == otherMatrix.size());
		for (unsigned int i = 0; i < size_; ++i) {
			elements_[i] += static_cast<T>(otherMatrix[i]);
		}
		return *this;
	}

	Matrix& operator+=(const data_type& otherData) const noexcept {
		assert(size_ == otherData.size());
		for (unsigned int i = 0; i < size_; ++i) {
			elements_[i] += otherData[i];
		}
		return *this;
	}

	// arithmetic subtract - operator overloads
	Matrix& operator-=(const Matrix& otherMatrix) noexcept {
		for (unsigned int i = 0; i < size_; ++i) {
			elements_[i] -= otherMatrix[i];
		}
		return *this;
	}

	template <typename U>
	Matrix& operator-=(const Matrix<U>& otherMatrix) noexcept {
		assert(size_ == otherMatrix.size());
		for (unsigned int i = 0; i < size_; ++i) {
			elements_[i] -= static_cast<T>(otherMatrix[i]);
		}
		return *this;
	}

	Matrix& operator-=(const data_type& otherData) const noexcept {
		assert(size_ == otherData.size());
		for (unsigned int i = 0; i < size_; ++i) {
			elements_[i] -= otherData[i];
		}
		return *this;
	}

	// index operator
	const T& operator()(const size_t i) const noexcept { return elements_[i]; }

	T& operator()(const size_t i) noexcept { return elements_[i]; }

	const T& operator[](const size_t i) const noexcept { return elements_[i]; }

	T& operator[](const size_t i) noexcept { return elements_[i]; }

	// member functions

	// print as a 1D array
	void print_elements() const {
		for (const auto& el : elements_)
			std::println("{:.6f}", el);
	}

	void print_matrix() const {
		for (size_t i = 0; i < m_; ++i) {
			for (size_t j = 0; j < n_; ++j) {
				std::print("{} ", elements_[n_ * i + j]);
			}
			std::println();
		}
		std::println();
	}

	// property accessor
	const size_t size() const noexcept { return size_; }

	const auto shape() const noexcept { return std::make_pair(m_, n_); }

	const data_type& elements() const noexcept { return elements_; }

	const data_type& elements(size_t i) const noexcept { return elements_[i]; }

	// Operations

	// Matrix Multiply
	// do basic mat mult on two matrices.
	template <typename U, typename V>
	friend auto matMultBasic(const Matrix<U>& A, const Matrix<V>& B);

 protected:
	// protected members

 private:
	// private members
	size_t m_;
	size_t n_;
	size_t size_;
	data_type elements_;
};

/// @brief Binary Arithmetic Addition Overload
/// @return
template <typename U, typename V>
auto operator+(const Matrix<U>& lhs, const Matrix<V>& rhs) {
	assert(lhs.size() == rhs.size());
	Matrix<typename std::common_type<U, V>::type> result(lhs);
	return result += rhs;
}

/// @brief Binary Arithmetic Subtraction Overload
/// @return
template <typename U, typename V>
auto operator-(const Matrix<U>& lhs, const Matrix<V>& rhs) {
	assert(lhs.size() == rhs.size());
	Matrix<typename std::common_type<U, V>::type> result(lhs);
	return result -= rhs;
}

// pass array or vector here, and use span or mdspan[c++23] here to represent them as a matrix
// INPUT:  A -> MxP & B -> PxN
// RETURN: C -> M*N
// RETURN_TYPE: Matrix<typename std::common_type<U, V>::type>
// assert(A.P == B.P);
template <typename U, typename V>
auto matMultBasic(const Matrix<U>& A, const Matrix<V>& B) {

	// the resultant matrix should be converted to appropriate type
	using common_t = typename std::common_type<U, V>::type;

	const size_t m = A.m_;
	const size_t p = A.n_;
	// A.n_ == B.m_ --> common index to sum over for the innermost loop
	const size_t n = B.n_;

	// todo:
	// !!!  do you need to make this object here. /
	// !!!  no need, can simply construct this while returning /
	// !!!  and maybe save on space??? /
	// !!!  store result in an array and then construct eg. : return Matrix<common_t>(result,m_, n_);
	// Matrix<common_t> result(std::vector<common_t>(m*n, 0), m, n);
	Matrix<common_t> result(m, n);

	for (size_t i = 0; i < m; ++i) {
		for (size_t j = 0; j < n; ++j) {
			size_t res_idx = n * i + j;
			for (size_t k = 0; k < p; ++k) {
				size_t a_idx = p * i + k;
				size_t b_idx = n * k + j;

				result(res_idx) += A(a_idx) * B(b_idx);
			}
		}
	}
	return result;
}

// takes an array or vector here, and uses std::span as a non-owning reference to multiple two matrices.
// additionally takes the shape of the incoming matrices as a std::pair each.
// INPUT:  A -> MxP & B -> PxN
// RETURN: C -> M*N
// RETURN_TYPE: std::<typename std::common_type<U, V>::type>
// assert(A.P == B.P);
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