#pragma once

#include "em_traits.hpp"
#include "em_unsigned.hpp"
#include "traits_specialization.hpp"

#include <concepts>
#include <cstddef>

namespace ember {

template <typename T>
concept ember_type = requires(const T& value, const Unsigned& bits) {
    { ember_traits<T>::bitwidth(value) } -> std::convertible_to<std::size_t>;

    static_cast<Unsigned>(value);
    static_cast<T>(bits);
};

} // namespace ember