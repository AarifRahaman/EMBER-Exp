#include "em_random.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("uniform returns start when start equals stop",
          "[math][random][uniform]") {
    REQUIRE(ember::math::random::uniform<int>(5, 5) == 5);
    REQUIRE(ember::math::random::uniform<int>(0, 0) == 0);
    REQUIRE(ember::math::random::uniform<int>(-10, -10) == -10);
}

TEST_CASE("uniform generates values within positive range",
          "[math][random][uniform]") {
    for (int i = 0; i < 1000; ++i) {
        const auto value =
            ember::math::random::uniform<int>(10, 20);

        REQUIRE(value >= 10);
        REQUIRE(value < 20);
    }
}

TEST_CASE("uniform generates values within negative and mixed range",
          "[math][random][uniform]") {
    for (int i = 0; i < 1000; ++i) {
        const auto value =
            ember::math::random::uniform<int>(-8, 8);

        REQUIRE(value >= -8);
        REQUIRE(value < 8);
    }
}

TEST_CASE("random generates valid 8-bit signed values",
          "[math][random]") {
    for (int i = 0; i < 1000; ++i) {
        const auto value =
            ember::math::random::random<int>(8);

        REQUIRE(value >= -128);
        REQUIRE(value < 128);
    }
}

TEST_CASE("random generates valid 16-bit signed values",
          "[math][random]") {
    for (int i = 0; i < 1000; ++i) {
        const auto value =
            ember::math::random::random<int>(16);

        REQUIRE(value >= -32768);
        REQUIRE(value < 32768);
    }
}