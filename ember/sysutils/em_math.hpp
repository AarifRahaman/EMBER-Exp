#pragma once

#include <iostream>
#include <type_traits>

namespace ember {

namespace math {

// Base 2 Power
template <typename T>

requires(std::is_integral<T>::value) constexpr T pow2(const T p) {
    return 0x1 << p;
}

// Base 2 Logatirhm
template <typename T>

requires(std::is_integral<T>::value) constexpr T log2(const T p) {
    size_t exp = 0;
    while ((1 << exp) < p) {
        exp++;
    }
    return exp;
}

// Max
template <typename Ta, typename Tb> constexpr std::common_type_t<Ta, Tb> max(const Ta A, const Tb B) {
    using Ct = std::common_type_t<Ta, Tb>;
    return (static_cast<Ct>(A) > static_cast<Ct>(B)) ? static_cast<Ct>(A) : static_cast<Ct>(B);
}


// Min
template <typename Ta, typename Tb> constexpr std::common_type_t<Ta, Tb> min(const Ta A, const Tb B) {
    using Ct = std::common_type_t<Ta, Tb>;
    return (static_cast<Ct>(A) < static_cast<Ct>(B)) ? static_cast<Ct>(A) : static_cast<Ct>(B);
}

namespace twoc {

template <typename T>

requires(std::is_integral<T>::value) constexpr T upbound(const T bitwidth) {
    return pow2(bitwidth - 1) - 1;
}

template <typename T>

requires(std::is_integral<T>::value) constexpr T lobound(const T bitwidth) {
    return -ember::math::pow2(bitwidth - 1);
}

} // namespace twoc

} // namespace math

} // namespace ember
