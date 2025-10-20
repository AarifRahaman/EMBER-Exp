
#include <iostream>
#include <bitset>

#include "ember.hpp"

// Design Under Test
#include "adder_wreg.hpp"

#define TESTSIZE 100
#define BITWIDTH 8
#define SIMTIME  5

using test_t=ember::int32_t;

template<typename T>
bool threshold(const T& A, const T& B, const T& value)
{
    T delta;
    if((A-B) < 0) {
        delta = -(A-B);
    } 
    else {
        delta = (A-B);
    }
    return (delta <= value);
}

int main(int argc, char* argv[])
{
    debug::adder_wreg<test_t>* dut;
    test_t input_a, input_b;
    test_t output_c;

    dut = new debug::adder_wreg<test_t>("ADD", BITWIDTH, BITWIDTH);    // Create DUT
    dut->connect();
    ember::time_t<long unsigned> dut_delay = 1;                        // Expected delay
    std::vector<ember::ISaboteur*> reglist = dut->getSaboteurs();    // Get DUT saboteurs


    // Testbench settings
#ifdef INTEGER
    test_t maxRange = (0x1 << (BITWIDTH-1)) -1;
    test_t minRange = -(0x1 << (BITWIDTH-1));
#else
    test_t maxRange = test_t(10.0);
    test_t minRange = test_t(-10.0);
#endif

    // == CORE TESTBENCH ROUTINE == //
    for(size_t testit=0; testit<TESTSIZE; testit++) {
        // random inputs
        input_a = ember::math::random::uniform<test_t>(minRange, maxRange);
        input_b = ember::math::random::uniform<test_t>(minRange, maxRange);
        // expected output
        output_c = input_a + input_b;

        // Clear Faults
        for(size_t it=0; it<reglist.size(); it++) {
            reglist[it]->clearAllMasks();
        }

        // Generate Fault Masks
        ember::fault::list<ember::fault::model::seu> seulist;

        for(size_t fiter=0; fiter<FAULTNUM; fiter++) {
            ember::time_t<long unsigned> itime = static_cast<long unsigned>(ember::math::random::uniform(int(dut_delay.getSimTime()+1), int(SIMTIME-1)));
            size_t pos = ember::math::random::uniform((size_t)0, reglist.size()); // ember rand int uniform is in [start, end)
            ember::ISaboteur* loc = reglist[pos];
            ember::fault::seu_t<long unsigned> _seu(itime, loc);
            seulist.insert(_seu);
            std::cout << ">> Generated Fault: " << _seu << std::endl;
        }

        std::cout << ">> Printing Faults List:" << std::endl;
        seulist.print();

        // Simulation Routine
        for(ember::time_t<long unsigned> tick = 0; tick < SIMTIME; tick++) {

            if(!seulist.empty()) {
                while(!seulist.empty() && ((seulist.getHead()).time() == tick)) {
                    ember::fault::fault_t<ember::fault::model, long unsigned> _fault = seulist.getHead();
                    std::cout << ">> SimTime = " << tick << ": \033[1;31minjecting fault " << _fault << "\033[0m" << std::endl;
                    _fault.location()->genFaultMask(_fault.fmodel());
                    seulist.pop();
                }
            }

            // DUT Update
            dut->A.write(input_a);
            dut->B.write(input_b);
            dut->update();
            dut->eval();

            // Inject Faults
            for(size_t it=0; it<reglist.size(); it++) {
                ember::ISaboteur* reg = reglist[it];
                reg->applyAllFaults();
            }

            // Printout
            std::cout << ">> SimTime = " << tick << " inputs(" << input_a << ", " << input_b << ")" << std::endl;
            if(tick >= dut_delay)
            {
                if(dut->C.read() == output_c) {
                    test_t temp = dut->C.read();
                    std::cout << ">> Output(" << temp << ")" << std::endl;
                }
                else {
                    test_t temp = dut->C.read();
                    std::bitset<BITWIDTH+1> golden(output_c), error(temp);
                    std::cout << ">> \033[1;31mOutput(" << temp << ") / Expected(" << output_c << ")\033[0m" << std::endl;
                    std::cout << ">> Expected(" << golden << ")" << std::endl;
                    std::cout << ">> Received(" << error << ")" << std::endl;
#ifndef RUNONFAILS
                    std::cout << "##########################################################################################" << std::endl;
                    std::cout << "###" << std::endl;
                    std::cout << "### TEST " << testit << " FAILED" << std::endl;
                    std::cout << "###" << std::endl;
                    std::cout << "##########################################################################################" << std::endl;
                    return -1;
#endif
                }
            }
            else {
                std::cout << std::endl;
            }
        }

        std::cout << "##########################################################################################" << std::endl;
        std::cout << "###" << std::endl;
        std::cout << "### TEST " << testit << " DONE" << std::endl;
        std::cout << "###" << std::endl;
        std::cout << "##########################################################################################" << std::endl;
        seulist.clear();
    }

    std::cout << "=> ALL TESTS DONE" << std::endl;

    delete dut;

    return 0;
}
