#include "em_math.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("pow2 computes powers of 2", "[math][pow2]") {
    REQUIRE(ember::math::pow2(0) == 1);
    REQUIRE(ember::math::pow2(1) == 2);
    REQUIRE(ember::math::pow2(2) == 4);
    REQUIRE(ember::math::pow2(3) == 8);
    REQUIRE(ember::math::pow2(4) == 16);
}

TEST_CASE("log2 computes base-2 logarithm", "[math][log2]") {
    REQUIRE(ember::math::log2(1) == 0);
    REQUIRE(ember::math::log2(2) == 1);
    REQUIRE(ember::math::log2(5) == 3);
    REQUIRE(ember::math::log2(8) == 3);
    REQUIRE(ember::math::log2(13) == 4);
}

TEST_CASE("max returns the larger value", "[math][max]") {
    REQUIRE(ember::math::max(5, 5.9) == 5.9);
    REQUIRE(ember::math::max(3, 5) == 5);
    REQUIRE(ember::math::max(7, 7) == 7);
}

TEST_CASE("min returns the smaller value", "[math][min]") {
    REQUIRE(ember::math::min(5, 3) == 3);
    REQUIRE(ember::math::min(3, 5) == 3);
    REQUIRE(ember::math::min(7, 7) == 7);
}

TEST_CASE("twoc upper bound computes maximum signed value", "[math][twoc][upbound]") {
    REQUIRE(ember::math::twoc::upbound(8) == 127);
    REQUIRE(ember::math::twoc::upbound(4) == 7);
}

TEST_CASE("twoc lower bound computes minimum signed value", "[math][twoc][lobound]") {
    REQUIRE(ember::math::twoc::lobound(8) == -128);
    REQUIRE(ember::math::twoc::lobound(4) == -8);
}