#pragma once

#include "em_types.hpp"

#include <random>

namespace ember {

namespace math {

namespace random {

// Random INT of bitwidth
template <typename T> T random(const size_t& bitwidth) requires(std::integral<T>);

// Uniform INT in a Range [start, stop)
template <typename T> T uniform(const T& start, const T& stop) requires(std::integral<T>);

} // namespace random

} // namespace math

} // namespace ember

#include "tpp/em_random.tpp"
