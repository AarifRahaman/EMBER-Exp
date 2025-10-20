
#include <iostream>

#include "ember.hpp"
#include "ember_saboteurs.hpp"

#define WIDTH 4
#define DEPTH 10
#define BITWIDTH 8

using test_t = ember::int8_t;

int main(int argc, char* argv[])
{
    ember::time_t<> tick = 0;

    ember::saboteur::dpsram<test_t> mem(DEPTH, WIDTH, BITWIDTH);
    mem.we.bind();
    mem.wAddr.bind();
    mem.din.bind();
    mem.re.bind();
    mem.rAddr.bind();
    mem.dout.bind();

    // Mem Fill
    std::cout << "-I: Memory Writing..." << std::endl;
    for(size_t it=0; it<DEPTH; it++)
    {
        ember::array<test_t> aux(WIDTH, BITWIDTH, (it+1));

        mem.we.write(true);
        mem.re.write(false);
        mem.wAddr.write(it);
        mem.din.write(aux);

        std::cout << "-I: we = " << mem.we.read() << std::endl;
        std::cout << "-I: wAddr = " << mem.wAddr.read() << std::endl;
        std::cout << "-I: din = " << mem.din.read() << std::endl;
        std::cout << "-I: re = " << mem.re.read() << std::endl;
        std::cout << "-I: rAddr = " << mem.rAddr.read() << std::endl;
        std::cout << "-I: dout = " << mem.dout.read() << std::endl;
        std::cout << "---------------------------------------" << std::endl;

        mem.update();
        mem.eval();

        tick++;
    }

    // Mem Read
    std::cout << "-I: Memory Reading..." << std::endl;
    for(size_t it=0; it<DEPTH+1; it++)
    {
        ember::array<test_t> aux(WIDTH, BITWIDTH, 0);

        mem.we.write(false);
        mem.re.write(true);
        mem.rAddr.write(it);
        aux = mem.dout.read();

        std::cout << "-I: we = " << mem.we.read() << std::endl;
        std::cout << "-I: wAddr = " << mem.wAddr.read() << std::endl;
        std::cout << "-I: din = " << mem.din.read() << std::endl;
        std::cout << "-I: re = " << mem.re.read() << std::endl;
        std::cout << "-I: rAddr = " << mem.rAddr.read() << std::endl;
        std::cout << "-I: dout = " << mem.dout.read() << std::endl;
        std::cout << "---------------------------------------" << std::endl;

        mem.update();
        mem.eval();

        tick++;
    }

    std::cout << "-I: END OF TEST" << std::endl;
    return 0;
}