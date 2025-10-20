#ifndef __EMBER_RANDOM_TPP__
#define __EMBER_RANDOM_TPP__


//
// INT Random Bitwidth
//
template <typename T>
T ember::math::random::random(const size_t& bitwidth)
requires(std::integral<T>)
{
    const T upbound = static_cast<T>((0x1 << (bitwidth-1)));
    const T lobound = static_cast<T>(-(0x1 << (bitwidth-1)));

    return ember::math::random::uniform<T>(lobound, upbound);
}


//
// INT Uniform Random Generation
//
template <typename T>
T ember::math::random::uniform(const T& start, const T& stop)
requires(std::integral<T>)
{
    if(start == stop) { return start; }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(start, stop-1);
    return static_cast<T>(dis(gen));
}

#endif
