#include <bit>
#include <cstdint>
#include <catch2/catch_test_macros.hpp>

#include "ember.hpp"
#include "ember_saboteurs.hpp"

using test_t = ember::int8_t;
using ff_t   = ember::saboteur::flipflop<test_t>;
using seu    = ember::fault::seu_t;
using sa0    = ember::fault::sa0_t;
using sa1    = ember::fault::sa1_t;

static constexpr size_t BW = 8;


//This function counts how many bits are 1.

static unsigned ones (test_t v)
{
    return std::popcount (static_cast<unsigned char> (v));
}

TEST_CASE ("flipflop: update copies din to dout", "[flipflop]")
{
    ff_t ff (BW);
    ff.din.bind ();
    ff.dout.bind ();

    ff.din.write (5);
    ff.update ();
    REQUIRE (ff.dout.read () == 5);
}

TEST_CASE ("flipflop: reports its fault models and locations", "[flipflop]")
{
    ff_t ff (BW);
    ember::ISaboteurBase* base = &ff;

    REQUIRE (base->faultModelCount () == 3);
    REQUIRE (ff.locations (std::type_identity<seu>{}) == BW);
    REQUIRE (ff.locations (std::type_identity<sa0>{}) == BW);
    REQUIRE (ff.locations (std::type_identity<sa1>{}) == BW);
}

TEST_CASE ("flipflop: getSaboteurs returns itself", "[flipflop]")
{
    ff_t ff (BW);
    auto list = ff.getSaboteurs ();

    REQUIRE (list.size () == 1);
    REQUIRE (list[0] == static_cast<ember::ISaboteurBase*> (&ff));
}

TEST_CASE ("flipflop: SEU flips exactly one bit", "[flipflop]")
{
    ff_t ff (BW);
    ff.din.bind ();
    ff.dout.bind ();

    ff.din.write (0);
    ff.update ();

    ff.genFaultMask (std::type_identity<seu>{});
    ff.applyFault (std::type_identity<seu>{});

    REQUIRE (ones (ff.dout.read ()) == 1);
}

TEST_CASE ("flipflop: SA1 forces one bit to 1", "[flipflop]")
{
    ff_t ff (BW);
    ff.din.bind ();
    ff.dout.bind ();

    ff.din.write (0);
    ff.update ();

    ff.genFaultMask (std::type_identity<sa1>{});
    ff.applyFault (std::type_identity<sa1>{});

    REQUIRE (ones (ff.dout.read ()) == 1);
    REQUIRE (ff.locations (std::type_identity<sa1>{}) == BW - 1);
}

TEST_CASE ("flipflop: SA0 forces one bit to 0", "[flipflop]")
{
    ff_t ff (BW);
    ff.din.bind ();
    ff.dout.bind ();

    ff.din.write (static_cast<test_t> (-1)); // all bits 1
    ff.update ();

    ff.genFaultMask (std::type_identity<sa0>{});
    ff.applyFault (std::type_identity<sa0>{});

    REQUIRE (ones (ff.dout.read ()) == BW - 1);
    REQUIRE (ff.locations (std::type_identity<sa0>{}) == BW - 1);
}

TEST_CASE ("flipflop: stuck-at locations run out safely", "[flipflop]")
{
    ff_t ff (BW);

    for (size_t i = 0; i < BW + 3; i++)
        ff.genFaultMask (std::type_identity<sa1>{});

    REQUIRE (ff.locations (std::type_identity<sa1>{}) == 0);
}

TEST_CASE ("flipflop: clearAllMasks removes pending faults", "[flipflop]")
{
    ff_t ff (BW);
    ff.din.bind ();
    ff.dout.bind ();

    ff.din.write (0);
    ff.update ();

    ff.genFaultMask (std::type_identity<seu>{});
    ff.genFaultMask (std::type_identity<sa1>{});
    ff.clearAllMasks ();
    ff.applyAllFaults ();

    REQUIRE (ff.dout.read () == 0);
}

TEST_CASE ("flipflop: uniform selects it for a supported model", "[flipflop][uniform]")
{
    ff_t ff (BW);
    std::vector<ember::ISaboteurBase*> list = ff.getSaboteurs ();

    auto* picked = ember::fault::uniform<seu> (list);
    REQUIRE (picked == dynamic_cast<ember::ISaboteur<seu>*> (&ff));
}