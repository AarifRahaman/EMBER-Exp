#ifndef __EMBER_INTEGER_HPP__
#define __EMBER_INTEGER_HPP__

#include <iostream>
#include <cstdint>

namespace ember {

// Unsigned
using uint8_t=std::uint8_t;
using uint16_t=std::uint16_t;
using uint32_t=std::uint32_t;
using uint64_t=std::uint64_t;

// Signed
using int8_t=std::int8_t;
using int16_t=std::int16_t;
using int32_t=std::int32_t;
using int64_t=std::int64_t;

}

std::ostream& operator<<(std::ostream& os, ember::int8_t const& foo);


#endif
