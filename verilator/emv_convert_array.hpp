#ifndef __EMBER_VERILATOR_CONVERT_ARRAY_HPP__
#define __EMBER_VERILATOR_CONVERT_ARRAY_HPP__

#include "em_types.hpp"

namespace ember
{
    
namespace verilator
{

//
//* ========== NON EXTENDED - INTEGER ========== *//
//

template<typename vT, typename lT, size_t bitwidth>
requires(std::is_integral<vT>::value && std::is_integral<lT>::value)
constexpr void verilated2logic(const vT& vData, ember::array<lT>& lData, const size_t len)
{
    const lT m  = static_cast<lT>(1) << (bitwidth-1);

    for (size_t i=0; i<len; i++)
    {
        lData[i] = ((vData >> (i*bitwidth)) ^ m) - m;
    }
}

template<typename lT, typename vT, size_t bitwidth>
requires(std::is_integral<vT>::value && std::is_integral<lT>::value)
constexpr void logic2verilated(const ember::array<lT>& lData, vT& vData, const size_t len)
{
    const std::int64_t m  = (1U << bitwidth)-1;

    vData = 0;
    for(size_t i=0; i<len; i++) {
        vData |= ( lData[i] & m) << (i * bitwidth);
    }
}

//* ========== NON EXTENDED - FLOAT ========== *//
// TODO

//* ========== EXTENDED - INTEGER ========== *//
// TODO

//* ========== EXTENDED - FLOAT ========== *//
// TODO


} // namespace verilator

} // namespace ember

#endif