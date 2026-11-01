#pragma once
#include <cstddef>
#include <cstdint>

namespace ember {

class Unsigned {
private:
    std::uint64_t m_bits;
    const std::size_t m_bitwidth;

    void checkBitwidth(const Unsigned& other) const;

public:
    std::uint64_t get() const;
    void set(std::uint64_t value);
    std::size_t getBitwidth() const;

// Assignment Operators
Unsigned& operator&= (const Unsigned& other);
Unsigned& operator|= (const Unsigned& other);
Unsigned& operator^= (const Unsigned& other);

// Normal Operators
Unsigned operator& (const Unsigned& other) const;
Unsigned operator| (const Unsigned& other) const;
Unsigned operator^ (const Unsigned& other) const;

    
    
// CTOR    

explicit Unsigned(std::size_t bitwidth);

};

} // namespace ember
