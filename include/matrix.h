#include <array>
#include <vector>

// Matrix class using std::vector
// can be implemented, probably faster using compile time sizes and std::array.
// the size (or shape) will then have to be given as template parameters.
template <typename T>
class Matrix {
 public:
	// constructors

	// zero init with some given size
	constexpr Matrix(const size_t nX, const size_t nY) noexcept
			: m{nX}, n{nY}, size(nX * nY), elements(std::vector<T>(size, 0)) {
		//// size = m * n;
		//// elements = std::vector<T>(size, 0);	 // zero init the matrix
	}

	// init with given vector and shape
	constexpr Matrix(std::vector<T>& iVec, size_t nX, size_t nY) noexcept
			: m{nX}, n{nY}, size(m * n), elements(iVec) {
		//// size = m * n;
	}

	// using r-value with given vector and shape
	constexpr Matrix(std::vector<T>&& iVec, size_t nX, size_t nY) noexcept
			: m{nX}, n{nY}, size(m * n), elements(std::move(iVec)) {
		//// size = m * n;
		//// elements = std::move(iVec);
	}

	// using vector of different base type
	// todo:
	// !!! how to ensure this without a static assert?
	// !!! has to be the same size to copy
	template <typename T2>
	constexpr Matrix(std::vector<T2>& tVec, size_t nX, size_t nY) noexcept
			: m{nX}, n{nY}, size(m * n) {
		//// size = m * n;
		elements.resize(size);
		for (size_t i = 0; i < size; ++i)
			elements[i] = static_cast<T>(tVec[i]);
		// <typename std::common_type<T, T2>::type> does not!!? work here
		// since the matrix itself is type T and this would need to change that type.
	}

	// copy constructors
	// copy from Matrix of same type
	// todo: should we care about copies vs move?
	constexpr Matrix(const Matrix<T>& otherMatrix) noexcept
			: m{otherMatrix.m},
				n{otherMatrix.n},
				size(m * n),
				elements(otherMatrix.elements) {
		// // elements = otherMatrix.elements;
		// // m = otherMatrix.m;
		// // n = otherMatrix.n;
		// // size = m * n;
	}

	// copy from a matrix of a different type.
	// type converts to Matrix<T> from Matrix<U>
	template <typename U>
	constexpr Matrix(const Matrix<U>& otherMatrix) noexcept
			: m{otherMatrix.m},
				n{otherMatrix.n},
				size(m * n),
				elements(otherMatrix.elements.begin(), otherMatrix.elements.end()) {
		// // m = otherMatrix.m;
		// // n = otherMatrix.n;
		// // size = m * n;
	}

 public:
	//  overloads

	// index operator
	const T& operator()(const size_t i) const noexcept { return elements[i]; }

	T& operator()(const size_t i) noexcept { return elements[i]; }

	const T& operator[](const size_t i) const noexcept { return elements[i]; }

	T& operator[](const size_t i) noexcept { return elements[i]; }

	// member functions

	// print as a 1D array
	void print_elements() const {
		for (const auto& el : elements)
			std::cout << el << std::endl;
	}

	void print_matrix() const {
		for (size_t i = 0; i < m; ++i) {
			for (size_t j = 0; j < n; ++j) {
				std::cout << elements[n * i + j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	// Operations

	// Matrix Multiply

	// do basic mat mult on two matrices.
	template <typename U, typename V>
	friend auto matMultBasic(const Matrix<U>& A, const Matrix<V>& B);

 protected:
	// protected members

 private:
	// private members

 public:
	// member variables
	size_t m;
	size_t n;
	size_t size;
	std::vector<T> elements;
};

// pass array or vector here, and use span or mdspan[c++23] here to represent them as a matrix
// INPUT:  A -> MxP & B -> PxN
// RETURN: C -> M*N
// RETURN_TYPE: Matrix<typename std::common_type<U, V>::type>
// assert(A.P == B.P);
template <typename U, typename V>
auto matMultBasic(const Matrix<U>& A, const Matrix<V>& B) {

	// the resultant matrix should be converted to appropriate type
	using common_t = typename std::common_type<U, V>::type;

	const size_t m = A.m;
	const size_t p = A.n;
	// A.n == B.m --> common index to sum over for the innermost loop
	const size_t n = B.n;

	// todo:
	// !!!  do you need to make this object here. /
	// !!!  no need, can simply construct this while returning /
	// !!!  and maybe save on space??? /
	// !!!  store result in an array and then construct eg. : return Matrix<common_t>(result,m, n);
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
