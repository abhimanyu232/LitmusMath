#ifndef COMMON_H
#define COMMON_H

using float_type = double;

// todo:
template <typename T>
constexpr T KahanSum(const T& a, const T& b) {
  return a + b;
}

template <typename T>
constexpr T PairwiseSum(const T& a, const T& b) {
  return a + b;
}

#endif