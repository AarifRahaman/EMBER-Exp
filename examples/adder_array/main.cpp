
#include <iostream>
#include <bitset>

#include "ember.hpp"

// Design Under Test
#include "adder_array.hpp"

#define TESTSIZE 100
#define ALUNUM   4
#define BITWIDTH 8
#define SIMTIME  5
#define FAULTNUM 0

const bool TESTSEU = false;
const bool TESTSA0 = false;
const bool TESTSA1 = true;

using test_t = ember::int32_t;


int main(int argc, char* argv[])
{
    debug::adder_array<test_t>* dut;
    const ember::time_t<long unsigned> delay = 1;
    ember::array<test_t> input_a(ALUNUM);
    ember::array<test_t> input_b(ALUNUM);
    ember::array<test_t> output_c(ALUNUM);

    dut = new debug::adder_array<test_t>(ALUNUM, BITWIDTH, BITWIDTH);
    dut->connect();


    // GET REGLIST
    std::vector<ember::ISaboteur*> reglist = dut->getSaboteurs();

    // TEST ADDER_WREG
    for(size_t testit=0; testit<TESTSIZE; testit++) {
        for(size_t it=0; it<ALUNUM; it++) {
            // random inputs
            input_a[it] = ember::math::random::random<test_t>(BITWIDTH);
            input_b[it] = ember::math::random::random<test_t>(BITWIDTH);
            // expected output
            output_c[it] = input_a[it] + input_b[it];
        }

        // Clear Faults
        for(size_t it=0; it<reglist.size(); it++) {
            reglist[it]->clearAllMasks();
        }

        // GENERATE FAULTS
        ember::fault::list<ember::fault::model::seu> seulist;
        ember::fault::list<ember::fault::model::sa0> sa0list;
        ember::fault::list<ember::fault::model::sa1> sa1list;

        if(TESTSEU) {
            for(size_t fiter=0; fiter<FAULTNUM; fiter++) {
                ember::time_t<long unsigned> itime = static_cast<long unsigned>(ember::math::random::uniform(static_cast<long unsigned>(delay.getSimTime()+1), static_cast<long unsigned>(SIMTIME-1)));
                size_t pos = ember::math::random::uniform((size_t)0, reglist.size()-1);
                ember::ISaboteur* loc = reglist[pos];
                ember::fault::seu_t _seu(itime, loc);
                seulist.insert(_seu);
                std::cout << ">> Generated Fault: " << _seu << std::endl;
            }
        }

        if(TESTSA0) {
            for(size_t fiter=0; fiter<FAULTNUM; fiter++) {
                ember::time_t<long unsigned> itime = static_cast<long unsigned>(0);
                size_t pos = ember::math::random::uniform((size_t)0, reglist.size()-1);
                ember::ISaboteur* loc = reglist[pos];
                ember::fault::sa0_t _sa0(itime, loc);
                sa0list.insert(_sa0);
                std::cout << ">> Generated Fault: " << _sa0 << std::endl;
            }
        }

        if(TESTSA1) {
            for(size_t fiter=0; fiter<FAULTNUM; fiter++) {
                ember::time_t<long unsigned> itime = static_cast<long unsigned>(0);
                size_t pos = ember::math::random::uniform((size_t)0, reglist.size()-1);
                ember::ISaboteur* loc = reglist[pos];
                ember::fault::sa1_t _sa1(itime, loc);
                sa1list.insert(_sa1);
                std::cout << ">> Generated Fault: " << _sa1 << std::endl;
            }
        }

        std::cout << ">> Printing Seu List:" << std::endl;
        seulist.print();
        std::cout << ">> Printing Sa0 List:" << std::endl;
        sa0list.print();
        std::cout << ">> Printing Sa1 List:" << std::endl;
        sa1list.print();

        // Simulation
        for(ember::time_t<long unsigned> tick = 0; tick < SIMTIME; tick++) {

            if(!seulist.empty()) {
                while(!seulist.empty() && ((seulist.getHead()).time() == tick)) {
                    ember::fault::fault_t<ember::fault::model, long unsigned> _fault = seulist.getHead();
                    std::cout << ">> SimTime = " << tick << ": \033[1;31minjecting fault " << _fault << "\033[0m" << std::endl;
                    _fault.location()->genFaultMask(_fault.fmodel());
                    seulist.pop();
                }
            }
            if(!sa0list.empty()) {
                while(!sa0list.empty() && ((sa0list.getHead()).time() == tick)) {
                    ember::fault::fault_t<ember::fault::model, long unsigned> _fault = sa0list.getHead();
                    std::cout << ">> SimTime = " << tick << ": \033[1;31minjecting fault " << _fault << "\033[0m" << std::endl;
                    _fault.location()->genFaultMask(_fault.fmodel());
                    sa0list.pop();
                }
            }
            if(!sa1list.empty()) {
                while(!sa1list.empty() && ((sa1list.getHead()).time() == tick)) {
                    ember::fault::fault_t<ember::fault::model, long unsigned> _fault = sa1list.getHead();
                    std::cout << ">> SimTime = " << tick << ": \033[1;31minjecting fault " << _fault << "\033[0m" << std::endl;
                    _fault.location()->genFaultMask(_fault.fmodel());
                    sa1list.pop();
                }
            }

            // DUT Update
            dut->A.write(input_a);
            dut->B.write(input_b);
            dut->update();
            dut->eval();

            // Fault Insertion
            for(size_t it=0; it<reglist.size(); it++) {
                reglist[it]->applyAllFaults();
            }

            // Printout
            std::cout << ">> input_a = " << input_a << std::endl;
            std::cout << ">> input_b = " << input_b << std::endl;
            
            if(tick >= delay) {
                if(dut->C.read() == output_c) {
                    std::cout << ">> output = " << dut->C.read() << std::endl;
                }
                else {
                    std::cout << ">> \033[1;31moutput = " << dut->C.read() << "\033[0m" << std::endl;
                    std::cout << ">> Expected = " << output_c << std::endl;
                }
            }
        }

        std::cout << ">> TEST " << testit << " DONE" << std::endl;
        seulist.clear();
    }

    std::cout << "=> ALL TESTS DONE" << std::endl;

    delete dut;

    return 0;
}