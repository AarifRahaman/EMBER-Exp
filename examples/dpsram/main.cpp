#include <iostream>
#include <type_traits>

#include "ember.hpp"
#include "ember_saboteurs.hpp"

#define WIDTH 4
#define DEPTH 10
#define BITWIDTH 8

using test_t = ember::int8_t;

int main (int argc, char* argv[])
{
    ember::sim::Tick tick = 0;

    ember::saboteur::dpsram<test_t> mem (DEPTH, WIDTH, BITWIDTH);
    mem.we.bind ();
    mem.wAddr.bind ();
    mem.din.bind ();
    mem.re.bind ();
    mem.rAddr.bind ();
    mem.dout.bind ();

    // Mem Fill
    std::cout << "-I: Memory Writing..." << std::endl;
    for (size_t it = 0; it < DEPTH; it++)
    {
        ember::array<test_t> aux (WIDTH, BITWIDTH, (it + 1));

        mem.we.write (true);
        mem.re.write (false);
        mem.wAddr.write (it);
        mem.din.write (aux);

        std::cout << "-I: we = " << mem.we.read () << std::endl;
        std::cout << "-I: wAddr = " << mem.wAddr.read () << std::endl;
        std::cout << "-I: din = " << mem.din.read () << std::endl;
        std::cout << "-I: re = " << mem.re.read () << std::endl;
        std::cout << "-I: rAddr = " << mem.rAddr.read () << std::endl;
        std::cout << "-I: dout = " << mem.dout.read () << std::endl;
        std::cout << "---------------------------------------" << std::endl;

        mem.update ();
        mem.eval ();

        tick++;
    }

    // Mem Read
    std::cout << "-I: Memory Reading..." << std::endl;
    for (size_t it = 0; it < DEPTH + 1; it++)
    {
        ember::array<test_t> aux (WIDTH, BITWIDTH, 0);

        mem.we.write (false);
        mem.re.write (true);
        mem.rAddr.write (it);
        aux = mem.dout.read ();

        std::cout << "-I: we = " << mem.we.read () << std::endl;
        std::cout << "-I: wAddr = " << mem.wAddr.read () << std::endl;
        std::cout << "-I: din = " << mem.din.read () << std::endl;
        std::cout << "-I: re = " << mem.re.read () << std::endl;
        std::cout << "-I: rAddr = " << mem.rAddr.read () << std::endl;
        std::cout << "-I: dout = " << mem.dout.read () << std::endl;
        std::cout << "---------------------------------------" << std::endl;

        mem.update ();
        mem.eval ();

        tick++;
    }

        // --- Saboteur interface test ---
        ember::ISaboteurBase* base = &mem;
        std::cout << "-I: faultModelCount = " << base->faultModelCount() << std::endl;

        if (auto* s = dynamic_cast<ember::ISaboteur<ember::fault::seu_t>*>(base))
            std::cout << "-I: seu_t supported" << std::endl;
        if (auto* s = dynamic_cast<ember::ISaboteur<ember::fault::sa0_t>*>(base))
            std::cout << "-I: sa0_t supported" << std::endl;
        if (auto* s = dynamic_cast<ember::ISaboteur<ember::fault::sa1_t>*>(base))
            std::cout << "-I: sa1_t supported" << std::endl;


        // --- SEU injection test ---
        auto dump = [&] (const char* label)
        {
            std::cout << "-I: " << label << std::endl;
            mem.we.write (false);
            mem.re.write (true);
            for (size_t a = 0; a < DEPTH; a++)
            {
                mem.rAddr.write (a);
                mem.update ();
                mem.eval ();
                std::cout << "   addr " << a << " = " << mem.dout.read () << std::endl;
            }
        };

        dump ("memory BEFORE SEU fault");

        using seu = ember::fault::seu_t;
        if (auto* s = dynamic_cast<ember::ISaboteur<seu>*> (base))
        {
            std::cout << "-I: SEU locations = " << s->locations (std::type_identity<seu>{}) << std::endl;
            s->genFaultMask (std::type_identity<seu>{});
            s->applyFault (std::type_identity<seu>{});
        }

        dump ("memory AFTER SEU fault");


        // --- SA1 injection test ---
        using sa1 = ember::fault::sa1_t;
        if (auto* s = dynamic_cast<ember::ISaboteur<sa1>*> (base))
        {
            std::cout << "-I: SA1 locations = " << s->locations (std::type_identity<sa1>{}) << std::endl;
            s->genFaultMask (std::type_identity<sa1>{});
            s->applyFault (std::type_identity<sa1>{});
        }
        dump ("memory AFTER SA1 fault");


        // --- SA0 injection test ---
        using sa0 = ember::fault::sa0_t;
        if (auto* s = dynamic_cast<ember::ISaboteur<sa0>*> (base))
        {
            std::cout << "-I: SA0 locations = " << s->locations (std::type_identity<sa0>{}) << std::endl;
            s->genFaultMask (std::type_identity<sa0>{});
            s->applyFault (std::type_identity<sa0>{});
            std::cout << "-I: SA0 locations after = " << s->locations (std::type_identity<sa0>{}) << std::endl;
        }
        dump ("memory AFTER SA0");

        // --- uniform selection test ---
        std::vector<ember::ISaboteurBase*> saboteurs = mem.getSaboteurs ();
        std::cout << "-I: saboteurs in list = " << saboteurs.size () << std::endl;

        auto* picked = ember::fault::uniform<ember::fault::seu_t> (saboteurs);
        std::cout << "-I: uniform returned dpsram? " << (picked == dynamic_cast<ember::ISaboteur<ember::fault::seu_t>*> (&mem)) << std::endl;

        // empty list must give nullptr
        std::vector<ember::ISaboteurBase*> none;
        std::cout << "-I: uniform on empty list is null? " << (ember::fault::uniform<ember::fault::seu_t> (none) == nullptr) << std::endl;

        std::cout << "-I: END OF TEST" << std::endl;
    return 0;
}
