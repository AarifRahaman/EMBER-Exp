
#include <iostream>
#include "ember.hpp"
#include "ember_saboteurs.hpp"

#include "netlist.hpp"

#define TESTNUM  100
#define COOLDOWN 2
#define DELAY    2

bool boolean_check(const bool A, const bool B, const bool C, const bool D)
{
    return (A & B) | !C | D;
}


int main(int argc, char* argv[])
{
    netlist    dut("dut");

    for(size_t testit=0; testit<TESTNUM; testit++)
    {
        bool a = rand() % 2;
        bool b = rand() % 2;
        bool c = rand() % 2;
        bool d = rand() % 2;

        std::cout << "====================================" << std::endl;
        std::cout << "== Test " << testit << " Starting..." << std::endl;
        std::cout << "====================================" << std::endl;
        for(ember::time_t<> tick=0; tick<(DELAY+COOLDOWN); tick++)
        {
            // Writing Bin DUT
            dut.a.write(a);
            dut.b.write(b);
            dut.c.write(c);
            dut.d.write(d);

            // Update DUT
            dut.update();

            // Eval DUT
            dut.eval();

            // Monitor
            std::cout << "-I: Inputs(A=" << a << ", B=" << b << ", C=" << c << ", D=" << d << ")" << std::endl;
            std::cout << "-I: Expected output(" << boolean_check(a, b, c, d) << ")" << std::endl;
            std::cout << "-I: Binary output(" << dut.out.read() << ")" << std::endl;

            // Assert
            if(tick > DELAY)
            {
                if(!(boolean_check(a, b, c, d) == dut.out.read()))
                {
                    std::cout << "-E: Binary DUT output is not the expected" << std::endl;
                    std::cout << "====================================" << std::endl;
                    std::cout << "== Test " << testit << " FAILED" << std::endl;
                    std::cout << "====================================" << std::endl;
                    return -1;
                }
            }
        }

        std::cout << "====================================" << std::endl;
        std::cout << "== Test " << testit << " PASSED" << std::endl;
    }

    std::cout << "====================================" << std::endl;
    std::cout << "== ALL TESTS PASSED" << std::endl;
    std::cout << "====================================" << std::endl;

    return 0;
}