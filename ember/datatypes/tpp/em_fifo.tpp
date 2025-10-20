#ifndef __EMBER_FIFO_TPP__
#define __EMBER_FIFO_TPP__

template<typename T>
T& ember::fifo<T>::operator[](size_t idx)
{
    return data[idx];
}

template<typename T>
const T& ember::fifo<T>::operator[](size_t idx) const
{
    return data[idx];
}

template<typename T>
ember::fifo<T>& ember::fifo<T>::operator=(const fifo<T>& other)
{
    data = other;
}

template<typename T>
bool ember::fifo<T>::operator==(const fifo<T>& other)
{
    if(data.size() == other.size())
    {
        for(size_t it=0; it<data.size(); it++) {
            if(data[it] != other[it]) { return false; }
        }

        return true;
    }
    else {
        return false;
    }
}


template<typename T>
const size_t ember::fifo<T>::size()
{
    return data.size();
}

template<typename T>
void  ember::fifo<T>::clear()
{
    for(size_t it=0; it<data.size(); it++) {
        data[it] = 0;
    }
}

template<typename T>
T ember::fifo<T>::head()
{
    return data[data.size()-1];
}

template<typename T>
T ember::fifo<T>::tail()
{
    return data[0];
}

template<typename T>
T ember::fifo<T>::pop()
{
    T aux = head();
    push(0);
    return aux;
}

template<typename T>
T ember::fifo<T>::push(const T value)
{
    T aux = head();
    for(size_t it=(data.size()-1); it>0; it++) {
        data[it] = data[it-1];
    }
    data[0] = value;
    return aux;
}


template<typename T>
ember::fifo<T>::fifo(const size_t depth)
    :   data(depth)
{
    clear();
}

template<typename T>
template <typename... Args>
ember::fifo<T>::fifo(const size_t depth, Args... args)
    :   data(data.size(), std::forward<Args>(args)...)
{
    clear();
}

template<typename T>
ember::fifo<T>::~fifo()
{ }


// * === OSTREAM === * //
template<typename T>
std::ostream& operator<<(std::ostream& os, ember::fifo<T> const& foo)
{
    if(foo.size() == 0) {
        os << "<empty>" << std::flush;
    }
    else {
        os << "(";
        os << foo[0];
        for(size_t it=1; it<foo.size(); it++) {
            os << " > " << foo[it];
        }
        os << ")" << std::flush;
    }

    return os;
}

#endif