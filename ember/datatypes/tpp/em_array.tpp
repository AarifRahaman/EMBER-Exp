#ifndef __EMBER_ARRAY_TPP__
#define __EMBER_ARRAY_TPP__

// * === METHODS === * //
template<typename T>
const size_t ember::array<T>::size() const
{
    return _len;
}

template<typename T>
const size_t ember::array<T>::singleBw() const
{
    return _bitwidth;
}

template<typename T>
const size_t ember::array<T>::totalBw() const
{
    return _len * _bitwidth;
}
 

template<typename T>
T& ember::array<T>::operator[](size_t idx)
{
    return data[idx];
}

template<typename T>
const T& ember::array<T>::operator[](size_t idx) const
{
    return data[idx];
}

template<typename T>
ember::array<T>& ember::array<T>::operator=(const array<T>& other)
{
    if(this == &other) { return *this; }
    if(_len != other.size()) {
        std::cerr << "-E(" << __FILE__ << "):" << __func__ << std::endl;
        throw std::logic_error("Not implemented feature");
    }

    _len = other.size();
    _bitwidth = other._bitwidth;
    memcpy(other.data);

    return *this;
}

template<typename T>
bool ember::array<T>::operator==(const ember::array<T>& other)
{
    // if((_len == other.size()) && (_bitwidth == other.singleBw()))
    if(_len == other.size())
    {
        for(size_t it=0; it<_len; it++) {
            if(data[it] != other[it]) { return false; }
        }

        return true;
    }
    else {
        return false;
    }
}


template<typename T>
void ember::array<T>::memset(const T& value)
{
    std::fill(&data[0], &data[_len], value);
}

template<typename T>
void ember::array<T>::memcpy(const T* other)
{
    std::copy(&other[0], &other[_len], &data[0]);
}

// FIXME: Bugged
template<typename T>
void ember::array<T>::cpArray(T* other) const
{
    std::copy(&data[0], &data[_len], &other[0]);
}


// * === CTORS === * //
template<typename T>
ember::array<T>::array(const size_t len)
    :   ember::array<T>::array(len, bitsizeof(T))
{ }

template<typename T>
ember::array<T>::array(const size_t len, const size_t bitwidth)
    :   _len(len),
        _bitwidth(bitwidth)
{
    data = new T[len];
}

template<typename T>
ember::array<T>::array(const size_t len, const size_t bitwidth, const T& value)
    :   ember::array<T>::array(len, bitwidth)
{
    memset(value);
}

template<typename T>
ember::array<T>::~array()
{
    if (data) delete[] data;
}


// * === OSTREAM === * //
template<typename T>
std::ostream& operator<<(std::ostream& os, ember::array<T> const& foo)
{
    if(foo.size() == 0) {
        os << "<empty>" << std::flush;
    }
    else {
        os << "(";
        os << foo[0];
        for(size_t it=1; it<foo.size(); it++) {
            os << ", " << foo[it];
        }
        os << ")" << std::flush;
    }

    return os;
}


template<typename T>
ember::array<T>::array(array&& other) noexcept
    : _len(other._len), _bitwidth(other._bitwidth), data(other.data)
{
    other.data = nullptr;
    other._len = 0;
    other._bitwidth = 0;
}

template<typename T>
ember::array<T>& ember::array<T>::operator=(array<T>&& other) noexcept
{
    if (this != &other) {
        delete[] data;
        _len = other._len;
        _bitwidth = other._bitwidth;
        data = other.data;

        other.data = nullptr;
        other._len = 0;
        other._bitwidth = 0;
    }
    return *this;
}

template<typename T>
ember::array<T>::array(const array<T>& other)
    : _len(other._len), _bitwidth(other._bitwidth)
{
    data = new T[_len];
    std::copy(other.data, other.data + _len, data);
}

#endif
