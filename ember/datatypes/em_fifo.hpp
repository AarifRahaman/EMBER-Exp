#ifndef __EMBER_FIFO_HPP__
#define __EMBER_FIFO_HPP__

#include <vector>

namespace ember
{

template<typename T>
class fifo
{
protected:
    const size_t len;
    std::vector<T> data;

public:

    const size_t size();
    void clear();

    T& operator[](size_t idx);
    const T& operator[](size_t idx) const;
    fifo<T>& operator=(const fifo<T>& other);
    bool operator==(const fifo<T>& other);

    T head();
    T tail();
    T pop();
    T push(const T value);

    fifo(const size_t depth);
    template <typename... Args>
    fifo(const size_t depth, Args... args);
    ~fifo();
};
    
} // namespace ember

template<typename T>
std::ostream& operator<<(std::ostream& os, ember::fifo<T> const& foo);


#include "tpp/em_fifo.tpp"

#endif