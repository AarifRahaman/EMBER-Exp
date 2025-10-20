
#include "em_integer.hpp"

std::ostream& operator<<(std::ostream& os, ember::int8_t const& foo)
{
    os << static_cast<int>(foo);
    return os;
}