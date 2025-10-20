#ifndef __MAC_HPP__
#define __MAC_HPP__

namespace debug {

void mac(
    ember::array<std::int8_t>& dat,
    ember::array<std::int8_t>& wt,
    int&               psums,
    const size_t       size)
{
    psums = 0;
    for(size_t c=0; c<size; c++)
    {
        psums += dat[c] * wt[c];
    }
}

}

#endif