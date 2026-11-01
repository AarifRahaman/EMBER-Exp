#include "ember/datatypes/em_unsigned.hpp"
#include "ember/errors/em_errors.hpp"
#include "ember/datatypes/em_concepts.hpp"
#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

TEST_CASE("Unsigned constructor stores bitwidth") {
    ember::Unsigned value(8);

    REQUIRE(value.getBitwidth() == 8);
    REQUIRE(value.get() == 0);
}

TEST_CASE("Unsigned value can be changed") {
    ember::Unsigned value(8);

    value.set(25);

    REQUIRE(value.get() == 25);
}

TEST_CASE("Unsigned rejects invalid bitwidth") {
    REQUIRE_THROWS_AS(ember::Unsigned(0), ember::InvalidArgument);
    REQUIRE_THROWS_AS(ember::Unsigned(65), ember::NotImplementedError);

    REQUIRE_NOTHROW(ember::Unsigned(1));
    REQUIRE_NOTHROW(ember::Unsigned(64));
}

// assignment operators

TEST_CASE("Unsigned AND assignment operator") {
    ember::Unsigned first(8);
    ember::Unsigned second(8);

    first.set(12);   // 1100
    second.set(10);  // 1010

    first &= second;

    REQUIRE(first.get() == 8); // 1000
}

TEST_CASE("Unsigned OR assignment operator") {
    ember::Unsigned first(8);
    ember::Unsigned second(8);

    first.set(12);   // 1100
    second.set(10);  // 1010

    first |= second;

    REQUIRE(first.get() == 14); // 1110
}

TEST_CASE("Unsigned XOR assignment operator") {
    ember::Unsigned first(8);
    ember::Unsigned second(8);

    first.set(12);   // 1100
    second.set(10);  // 1010

    first ^= second;

    REQUIRE(first.get() == 6); // 0110
}

//Normal Operators

TEST_CASE("Unsigned normal bitwise operators") {
    ember::Unsigned first(8);
    ember::Unsigned second(8);

    first.set(12);
    second.set(10);

    ember::Unsigned andResult = first & second;
    ember::Unsigned orResult = first | second;
    ember::Unsigned xorResult = first ^ second;

    REQUIRE(andResult.get() == 8);
    REQUIRE(orResult.get() == 14);
    REQUIRE(xorResult.get() == 6);

    // Normal operators must not modify the original objects.
    REQUIRE(first.get() == 12);
    REQUIRE(second.get() == 10);
}

//Bitwidth Check

TEST_CASE("Bitwise operations reject different bitwidths") {
    ember::Unsigned first(8);
    ember::Unsigned second(16);

    REQUIRE_THROWS_AS(first &= second, ember::BitwidthMismatch);
    REQUIRE_THROWS_AS(first |= second, ember::BitwidthMismatch);
    REQUIRE_THROWS_AS(first ^= second, ember::BitwidthMismatch);

    REQUIRE_THROWS_AS(first & second, ember::BitwidthMismatch);
    REQUIRE_THROWS_AS(first | second, ember::BitwidthMismatch);
    REQUIRE_THROWS_AS(first ^ second, ember::BitwidthMismatch);
}

TEST_CASE("Unsigned satisfies ember_type concept")
{
    STATIC_REQUIRE(ember::ember_type<ember::Unsigned>);
}

// Conversion of Unsigned to Custom_t and Custom to Unsigned

// Sample User defined data type
class MyData {
private:
    std::uint8_t bits;
    

public:
    
    std::uint8_t get() const {
        return bits;
    }

    // MyData -> Unsigned (Conversion operator belongs to source class). Remember conversion creates a new Unsigned object
    explicit operator ember::Unsigned() const {
        ember::Unsigned result(5);
        result.set(bits);
        return result;
    }

    // Unsigned -> MyData (Conversion CTOR belong to the destination class and here Mydata is destination class)
    explicit MyData(const ember::Unsigned& value)
        : bits(static_cast<std::uint8_t>(value.get() & 31)) {}

     
    //CTOR
    explicit MyData(std::uint8_t value)
        : bits(value & 31) {}

    
};

//Traits Specialization

template <> struct ember::ember_traits<MyData> {
    static std::size_t bitwidth(const MyData&) {
        return 5;
    }
};

TEST_CASE("User-defined datatype to Unsigned", "[Unsigned]")
{
    // Create user-defined datatype
    MyData userData(5);

    // Convert to EMBER Unsigned
    ember::Unsigned result =
        static_cast<ember::Unsigned>(userData);

    // Verify conversion
    REQUIRE(result.get() == 5);
    REQUIRE(result.getBitwidth() == 5);
}


TEST_CASE("Unsigned to user-defined datatype", "[Unsigned]")
{
    // Create EMBER Unsigned
    ember::Unsigned data(5);
    data.set(5);

    // Convert to user-defined datatype
    MyData result = static_cast<MyData>(data);

    // Verify conversion
    REQUIRE(result.get() == 5);
}

TEST_CASE("MyData satisfies ember concept")
{
STATIC_REQUIRE(ember::ember_type<MyData>);
}