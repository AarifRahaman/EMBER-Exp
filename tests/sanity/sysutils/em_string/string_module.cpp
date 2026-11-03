#include <catch2/catch_test_macros.hpp>

#include "em_string.hpp"

TEST_CASE("String utilities work correctly", "[string]") {

    SECTION("strcat concatenates two strings") {
        const char* result = ember::strcat("Hello", "World");

        // WARNING: relies on current implementation behavior
        REQUIRE(std::string(result) == "HelloWorld");
    }

    SECTION("hiercat concatenates hierarchy names") {
        REQUIRE(ember::hiercat("top", "child") == "top.child");
        REQUIRE(ember::hiercat("dut.alu", "out") == "dut.alu.out");
    }

    SECTION("hierseparator is dot") {
        REQUIRE(std::string(ember::hierseparator) == ".");
    }
}