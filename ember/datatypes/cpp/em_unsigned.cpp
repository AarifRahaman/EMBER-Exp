#include "em_unsigned.hpp"
#include "ember/errors/em_errors.hpp"
#include <stdexcept>

namespace ember {

std::uint64_t Unsigned::get() const {
    return m_bits;
}

void Unsigned::set(std::uint64_t value) {
    m_bits = value;
}

std::size_t Unsigned::getBitwidth() const {
    return m_bitwidth;
}

void Unsigned::checkBitwidth(const Unsigned& other) const {
     if (m_bitwidth != other.m_bitwidth) {
         throw ember::BitwidthMismatch("Bitwidth Must be same");
     }

}

// Assignment Operators

Unsigned& Unsigned::operator&=(const Unsigned& other) {
    checkBitwidth(other);
    m_bits &= other.m_bits;
    return *this;
}

Unsigned& Unsigned::operator|=(const Unsigned& other) {
    checkBitwidth(other);
    m_bits |= other.m_bits;
    return *this;
}

Unsigned& Unsigned::operator^=(const Unsigned& other) {
    checkBitwidth(other);
    m_bits ^= other.m_bits;
    return *this;
}

// NOrmal Operators

Unsigned Unsigned::operator&(const Unsigned& other) const{
    Unsigned result(*this);
    result &= other;
    return result;
}

Unsigned Unsigned::operator|(const Unsigned& other) const{
    Unsigned result(*this);
    result |= other;
    return result;
}

Unsigned Unsigned::operator^(const Unsigned& other) const{
    Unsigned result(*this);
    result ^= other;
    return result;
}


// CTOR

Unsigned::Unsigned(std::size_t bitwidth)
    : m_bits(0),
      m_bitwidth(bitwidth) {
        
    if (bitwidth == 0) {
    throw ember::InvalidArgument("Bitwidth cannot be 0.");
}

    if (bitwidth > 64) {
    throw ember::NotImplementedError(
        "Bitwidths larger than 64 are not implemented yet."
    );
}
}

} // namespace ember