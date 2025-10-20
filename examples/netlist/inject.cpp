
#include <iostream>
#include <vector>

#include "ember.hpp"
#include "ember_saboteurs.hpp"

#include "netlist.hpp"

#define TESTNUM  1000
#define FAULTNUM 50
#define DELAY    3

bool boolean_check(const bool A, const bool B, const bool C, const bool D)
{
    return (A & B) | !C | D;
}


int main(int argc, char* argv[])
{
    std::vector<float> sdc;
    netlist    dut("dut");
    std::vector<ember::ISaboteur*> saboteurs = dut.getSaboteurs();

    for(size_t faultit=0; faultit<FAULTNUM; faultit++)
    {
        std::cout << "====================================" << std::endl;
        std::cout << "== Test " << faultit << " Starting..." << std::endl;
        std::cout << "====================================" << std::endl;

        size_t err_cnt = 0;

        // Clear Faults
        for(size_t it=0; it<saboteurs.size(); it++) {
            saboteurs[it]->clearAllMasks();
        }

        // Generate Fault Masks

#ifdef SEU
        ember::ISaboteur* iloc = ember::fault::uniform(saboteurs, ember::fault::model::seu);
        ember::time_t<long unsigned> itime = static_cast<long unsigned>(ember::math::random::uniform(0, DELAY));
        ember::fault::seu_t<long unsigned> fault(itime, iloc);
#elif SA0
        ember::ISaboteur* iloc = ember::fault::uniform(saboteurs, ember::fault::model::sa0);
        ember::fault::sa0_t<long unsigned> fault(0, iloc);
#elif SA1
        ember::ISaboteur* iloc = ember::fault::uniform(saboteurs, ember::fault::model::sa1);
        ember::fault::sa1_t<long unsigned> fault(0, iloc);
#endif

        std::cout << ">> Generated Fault: " << fault << std::endl;

        for(size_t testit=0; testit<TESTNUM; testit++)
        {
            std::cout << ">> TEST " << testit << std::endl;

            bool a = rand() % 2;
            bool b = rand() % 2;
            bool c = rand() % 2;
            bool d = rand() % 2;


            // Simulation
            for(ember::time_t<> tick=0; tick<(DELAY); tick++)
            {
                // Writing Bin DUT
                dut.a.write(a);
                dut.b.write(b);
                dut.c.write(c);
                dut.d.write(d);

                // Update DUT
                dut.update();

                // Fault Injection
                if(tick == fault.time()) { fault.location()->genFaultMask(fault.fmodel()); }
                fault.location()->applyAllFaults();

                // Eval DUT
                dut.eval();

                

                // Monitor
                std::cout << "-I: Inputs(A=" << a << ", B=" << b << ", C=" << c << ", D=" << d << ")" << std::endl;
                std::cout << "-I: Expected output(" << boolean_check(a, b, c, d) << ")" << std::endl;
                std::cout << "-I: Binary output(" << dut.out.read() << ")" << std::endl;
            }

            if(dut.out.read() != boolean_check(a, b, c, d)) { err_cnt++; }
        }

        std::cout << "====================================" << std::endl;
        std::cout << "== Test " << faultit << " FINISH" << std::endl;
        std::cout << "== Error Rate = " << ((float)err_cnt / (float)TESTNUM) << " (" << err_cnt << "/" << TESTNUM << ")" << std::endl;
        sdc.push_back((float(err_cnt) / float(TESTNUM)));
    }

    std::cout << "====================================" << std::endl;
    std::cout << "== ALL TESTS DONE" << std::endl;
    for(size_t it=0; it<sdc.size(); it++) {
        std::cout << "== Test\t" << it << ": " << sdc[it] << std::endl;
    }
    std::cout << "====================================" << std::endl;

    return 0;
}