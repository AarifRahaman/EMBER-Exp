#include "FM_contracts.hpp"
#include <catch2/catch_test_macros.hpp>

struct SEU
{
    static constexpr std::string_view name = "SEU";  // view works like const char* and also can do comparison, constexpr makes the name known at compile time as our SEU is a fixed name.

    static constexpr ember::fault::FaultTiming timing = ember::fault::FaultTiming::transient; // Here timing is a member var and each of SEU object share the one timing, we can also do SEU::timing as we used static.

    
};


struct SA0
{
    static constexpr std::string_view name = "SA0";

    static constexpr ember::fault::FaultTiming timing = ember::fault::FaultTiming::permanent;

    
};


struct SA1
{
    static constexpr std::string_view name = "SA1";

    static constexpr ember::fault::FaultTiming timing = ember::fault::FaultTiming::permanent;

    
};

// This deliberately does NOT satisfy beFaultModel

struct FaultX
{
    static constexpr std::string_view name = "FaultX";

    using parameters = void;

};




//Compile time Test
TEST_CASE("beFaultModel concept")
{
    STATIC_REQUIRE(beFaultModel<SEU>);
    STATIC_REQUIRE(beFaultModel<SA0>);
    STATIC_REQUIRE(beFaultModel<SA1>);
    STATIC_REQUIRE(!beFaultModel<FaultX>);
}