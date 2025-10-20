#ifndef __EMBER_ARRAY_HPP__
#define __EMBER_ARRAY_HPP__

#include <algorithm>
#include <iostream>

#include "em_defs.hpp"

namespace ember
{

template<typename T>
class array
{
protected:
    size_t _len;
    size_t _bitwidth;

    T* data;

public:

    const size_t size() const;
    const size_t singleBw() const;
    const size_t totalBw() const; 

    T& operator[](size_t idx);
    const T& operator[](size_t idx) const;
    array<T>& operator=(const array<T>& other);
    bool operator==(const array<T>& other);

    void memset(const T& value);
    void memcpy(const T* other);
    void cpArray(T* other) const;

    array(const size_t len);
    array(const size_t len, const size_t bitwidth);
    array(const size_t len, const size_t bitwidth, const T& value);

    array(const array& other);

    // Move constructor
    array(array&& other) noexcept;

    // Move assignment operator
    array& operator=(array&& other) noexcept;

    ~array();
};

// Array Check
template <typename T>
struct is_array : std::false_type {};

template <typename T>
struct is_array<array<T>> : std::true_type {};


} // namespace ember

template<typename T>
std::ostream& operator<<(std::ostream& os, ember::array<T> const& foo);


#include "tpp/em_array.tpp"

#endif