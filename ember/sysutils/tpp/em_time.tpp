#ifndef __EMBER_TIME_TPP__
#define __EMBER_TIME_TPP__

//
// === TIME_T MEMBERS === //
//
template<typename backend_t>
requires(std::is_integral<backend_t>::value)
const backend_t ember::time_t<backend_t>::getSimTime() const {
    return _time;
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
const char* ember::time_t<backend_t>::getTimeUnit() const {
    return _utime.c_str();
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
void ember::time_t<backend_t>::reset() {
    _time = static_cast<backend_t>(0);
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
const ember::time_t<backend_t> ember::time_t<backend_t>::incr() {
    _time = _time + static_cast<backend_t>(1);

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << "-I(" << __FILE__ << "): [HEARTBEAT] _time = " << *this << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
#endif

    return *this;
}

//
// === ASSIGNMENT OPERATORS === //
//
template<typename backend_t>
requires(std::is_integral<backend_t>::value)
ember::time_t<backend_t>& ember::time_t<backend_t>::operator=(ember::time_t<backend_t> const& other) {
    if(this == &other) { return *this; }
    std::string unittime(other.getTimeUnit());
    _time = other.getSimTime();
    _utime = unittime;
    return *this;
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
ember::time_t<backend_t>& ember::time_t<backend_t>::operator=(T const other) {
    _time = static_cast<backend_t>(other);
    return *this;
}

//
// === INCREMENT OPERATORS === //
//
template<typename backend_t>
requires(std::is_integral<backend_t>::value)
ember::time_t<backend_t> ember::time_t<backend_t>::operator+(ember::time_t<backend_t> const& other) {
    // assert(!std::strcmp(_utime.c_str(), other.getTimeUnit()));
    ember::time_t<backend_t> sum(_time, _utime);
    sum = _time + other.getSimTime();
    return sum;
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
ember::time_t<backend_t>& ember::time_t<backend_t>::operator++() {
    this->incr();
    return *this;
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
ember::time_t<backend_t> ember::time_t<backend_t>::operator++( int ) {
    this->incr();
    return *this;
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
ember::time_t<backend_t>& ember::time_t<backend_t>::operator+=(ember::time_t<backend_t> const& other) {
    // assert(!std::strcmp(_utime.c_str(), other.getTimeUnit()));
    *this = _time + other.getSimTime();
    return *this;
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
ember::time_t<backend_t> ember::time_t<backend_t>::operator+(T const other) {
    ember::time_t<backend_t> value(static_cast<backend_t>(other), _utime);
    return this + value;
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
ember::time_t<backend_t>& ember::time_t<backend_t>::operator+=(T const other) {
    ember::time_t<backend_t> value(static_cast<backend_t>(other), _utime);
    return this += value;
}

//
// === DECREMENT OPERATORS === //
//
template<typename backend_t>
requires(std::is_integral<backend_t>::value)
ember::time_t<backend_t> ember::time_t<backend_t>::operator-(ember::time_t<backend_t> const& other) {
    // assert(!std::strcmp(_utime.c_str(), other.getTimeUnit()));
    ember::time_t<backend_t> sub(_time, _utime);
    sub = _time - other.getSimTime();
    return sub;
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
ember::time_t<backend_t>& ember::time_t<backend_t>::operator--() {
    _time = _time - static_cast<backend_t>(1);
    return *this;
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
ember::time_t<backend_t> ember::time_t<backend_t>::operator--( int ) {
    _time = _time - static_cast<backend_t>(1);
    return *this;
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
ember::time_t<backend_t>& ember::time_t<backend_t>::operator-=(ember::time_t<backend_t> const& other) {
    // assert(!std::strcmp(_utime.c_str(), other.getTimeUnit()));
    *this = _time - other.getSimTime();
    return *this;
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
ember::time_t<backend_t> ember::time_t<backend_t>::operator-(T const other) {
    ember::time_t<backend_t> value(static_cast<backend_t>(other), _utime);
    return this - value;
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
ember::time_t<backend_t>& ember::time_t<backend_t>::operator-=(T const other) {
    ember::time_t<backend_t> value(static_cast<backend_t>(other), _utime);
    return this -= value;
}

//
// === LOGICAL OPERATORS === //
//
template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
bool ember::time_t<backend_t>::operator==(ember::time_t<T> const& other) const {
    // assert(!std::strcmp(_utime.c_str(), other.getTimeUnit()));
    return (_time == other.getSimTime());
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
bool ember::time_t<backend_t>::operator!=(ember::time_t<T> const& other) const {
    // assert(!std::strcmp(_utime.c_str(), other.getTimeUnit()));
    return (_time != other.getSimTime());
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
bool ember::time_t<backend_t>::operator<(ember::time_t<T> const& other) const {
    // assert(!std::strcmp(_utime.c_str(), other.getTimeUnit()));
    return (_time < other.getSimTime());
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
bool ember::time_t<backend_t>::operator<=(ember::time_t<T> const& other) const {
    // assert(!std::strcmp(_utime.c_str(), other.getTimeUnit()));
    return (_time <= other.getSimTime());
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
bool ember::time_t<backend_t>::operator>(ember::time_t<T> const& other) const {
    // assert(!std::strcmp(_utime.c_str(), other.getTimeUnit()));
    return (_time >& other.getSimTime());
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
bool ember::time_t<backend_t>::operator>=(ember::time_t<T> const& other) const {
    // assert(!std::strcmp(_utime.c_str(), other.getTimeUnit()));
    return (_time >= other.getSimTime());
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
bool ember::time_t<backend_t>::operator==(T const& other) const {
    return (_time == static_cast<backend_t>(other));
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
bool ember::time_t<backend_t>::operator!=(T const& other) const {
    return (_time != static_cast<backend_t>(other));
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
bool ember::time_t<backend_t>::operator<(T const& other) const {
    return (_time < static_cast<backend_t>(other));
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
bool ember::time_t<backend_t>::operator<=(T const& other) const {
    return (_time <= static_cast<backend_t>(other));
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
bool ember::time_t<backend_t>::operator>(T const& other) const {
    return (_time > static_cast<backend_t>(other));
}

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
template<typename T>
requires(std::is_integral<T>::value)
bool ember::time_t<backend_t>::operator>=(T const& other) const {
    return (_time >= static_cast<backend_t>(other));
}

//
// === CTOR === //
//
template<typename backend_t>
requires(std::is_integral<backend_t>::value)
ember::time_t<backend_t>::time_t()
    :   _time(0),
        _utime("clk(s)")
{ }

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
ember::time_t<backend_t>::time_t(backend_t time)
    :   _time(time),
        _utime("clk(s)")
{ }

template<typename backend_t>
requires(std::is_integral<backend_t>::value)
ember::time_t<backend_t>::time_t(backend_t time, const char* utime)
    :   _time(time),
        _utime(utime)
{ }


//
// === OSTREAM === //
//
template<typename T>
std::ostream& operator<<(std::ostream& os, ember::time_t<T> const& foo) {
    os << foo.getSimTime() << " " << foo.getTimeUnit() << std::flush;
    return os;
}


#endif
