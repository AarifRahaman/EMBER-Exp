#include "em_traits.hpp"
#include "em_unsigned.hpp"

#include <cstddef>

namespace ember {

template <> struct ember_traits<Unsigned> {
    static std::size_t bitwidth(const Unsigned& value) {
        return value.getBitwidth();
    }
};

} // namespace ember