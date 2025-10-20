#ifndef __EMBER_TIME_HPP__
#define __EMBER_TIME_HPP__

#include <iostream>
#include <type_traits>
#include <cstring>
#include <cassert>

namespace ember {

template <class backend_t = long unsigned>
requires(std::is_integral<backend_t>::value)
class time_t {
protected:
    backend_t _time;
    std::string _utime;

public:
    const backend_t getSimTime() const;
    const char* getTimeUnit() const;
    void reset();
    const time_t incr();

    // Assignment Operators
    time_t& operator=(time_t const& other);

    template<typename T>
    requires(std::is_integral<T>::value)
    time_t& operator=(T const other);

    // Increment Operators
    time_t operator+(time_t const& other);
    time_t& operator++();
    time_t operator++( int );
    time_t& operator+=(time_t const& other);

    template<typename T>
    requires(std::is_integral<T>::value)
    time_t operator+(T const other);

    template<typename T>
    requires(std::is_integral<T>::value)
    time_t& operator+=(T const other);

    // Decrement Operators
    time_t operator-(time_t const& other);
    time_t& operator--();
    time_t operator--( int );
    time_t& operator-=(time_t const& other);

    template<typename T>
    requires(std::is_integral<T>::value)
    time_t operator-(T const other);

    template<typename T>
    requires(std::is_integral<T>::value)
    time_t& operator-=(T const other);

    // Logical Operators
    template<typename T>
    requires(std::is_integral<T>::value)
    bool operator==(time_t<T> const& other) const;

    template<typename T>
    requires(std::is_integral<T>::value)
    bool operator!=(time_t<T> const& other) const;

    template<typename T>
    requires(std::is_integral<T>::value)
    bool operator<(time_t<T> const& other) const;

    template<typename T>
    requires(std::is_integral<T>::value)
    bool operator<=(time_t<T> const& other) const;

    template<typename T>
    requires(std::is_integral<T>::value)
    bool operator>(time_t<T> const& other) const;

    template<typename T>
    requires(std::is_integral<T>::value)
    bool operator>=(time_t<T> const& other) const;

    template<typename T>
    requires(std::is_integral<T>::value)
    bool operator==(T const& other) const;

    template<typename T>
    requires(std::is_integral<T>::value)
    bool operator!=(T const& other) const;

    template<typename T>
    requires(std::is_integral<T>::value)
    bool operator<(T const& other) const;

    template<typename T>
    requires(std::is_integral<T>::value)
    bool operator<=(T const& other) const;

    template<typename T>
    requires(std::is_integral<T>::value)
    bool operator>(T const& other) const;

    template<typename T>
    requires(std::is_integral<T>::value)
    bool operator>=(T const& other) const;


    // CTOR
    time_t();
    // FIXME: Difficult resolving constructor overloading
    //time_t(const char* utime);
    time_t(backend_t time);
    time_t(backend_t time, const char* utime);
};

}

template<typename T>
std::ostream& operator<<(std::ostream& os, ember::time_t<T> const& foo);

#include "tpp/em_time.tpp"

#endif
