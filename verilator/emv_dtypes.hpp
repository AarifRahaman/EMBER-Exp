#ifndef __EMBER_VERILATOR_DTYPES_HPP__
#define __EMBER_VERILATOR_DTYPES_HPP__

#include <cstddef>

#include "em_defs.hpp"

namespace ember
{

namespace verilator
{

template<typename T>
struct is_verilated
{
public:
    static constexpr bool value = std::is_integral<T>::value;
};

template<typename T, size_t bitwidth>
requires(is_verilated<T>::value)
struct is_extended
{
public:
    static constexpr bool value = (bitwidth > bitsizeof(T));
};

template<typename T, size_t bitwidth>
requires(is_extended<T, bitwidth>::value)
struct extended
{
public:
    static constexpr size_t len = size_t(size_t(bitwidth) / size_t(bitsizeof(T))) + ((size_t(bitwidth) / size_t(bitsizeof(T)))? 1 : 0);
};

} // namespace verilator

} // namespace ember


#endif