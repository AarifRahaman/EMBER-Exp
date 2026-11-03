#include "FM_contracts.hpp"
#include "emf_models.hpp"
#include <catch2/catch_test_macros.hpp>



struct FaultX {};


//Compile time Test
TEST_CASE("FaultModel concept")
{
    STATIC_REQUIRE(FaultModel<ember::fault::seu_t>);
    STATIC_REQUIRE(FaultModel<ember::fault::sa0_t>);
    STATIC_REQUIRE(FaultModel<ember::fault::sa1_t>);
    STATIC_REQUIRE(!FaultModel<FaultX>);
}