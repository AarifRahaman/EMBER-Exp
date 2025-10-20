
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "ember.hpp"
#include "ember_verilator.hpp"

#include "include/mac.hpp"

#include <verilated.h>
#include <verilated_vcd_c.h>

#include "mac_wrapper.hpp"
#include "Vmac_mac.h"

#define TESTNUM 10
#define SIMTIME 10
#define BITWIDTH 8
#define FAULTNUM 1

int main(int argc, char* argv[])
{
    Verilated::commandArgs(argc, argv); // Initialize Verilator arguments

    ember::array<ember::int8_t> dat(_ATOMIC);
    ember::array<ember::int8_t> wt(_ATOMIC);
    int psums_check;

    mac_wrapper dut;
    dut.connect();
    std::vector<ember::ISaboteur*> reglist = dut.getSaboteurs();

    std::cout << "-I: Start running verilator test..." << std::endl;

    for(size_t testit=0; testit<TESTNUM; testit++)
    {
        // Clear Faults
        for(size_t it=0; it<reglist.size(); it++) {
            reglist[it]->clearAllMasks();
        }

        // Randomize Inputs
        for(size_t i=0; i<_ATOMIC; i++) {
            dat[i] = ember::math::random::random<ember::int8_t>(BITWIDTH);
            wt[i]  = ember::math::random::random<ember::int8_t>(BITWIDTH);
        }

        std::cout << ">> [test " << testit << "] dat:" << std::flush;
        for(size_t i=0; i<_ATOMIC; i++) { std::cout << " " << (int) dat[i] << std::flush; }
        std::cout << std::endl;

        std::cout << ">> [test " << testit << "] wt:" << std::flush;
        for(size_t i=0; i<_ATOMIC; i++) { std::cout << " " << (int) wt[i] << std::flush; }
        std::cout << std::endl;

        // Sets Inputs
        dut.valid.write(true);
        dut.dat.write(dat);
        dut.wt.write(wt);


        // Generate Fault Masks
        ember::fault::list<ember::fault::model::seu> seulist;

        for(size_t fiter=0; fiter<FAULTNUM; fiter++) {
            ember::time_t<long unsigned> itime = static_cast<long unsigned>(ember::math::random::uniform(int(0), int(SIMTIME)));
            size_t pos = ember::math::random::uniform((size_t)0, reglist.size()-1);
            ember::ISaboteur* loc = reglist[pos];
            ember::fault::seu_t _seu(itime, loc);
            seulist.insert(_seu);
            std::cout << ">> Generated Fault: " << _seu << std::endl;
        }

        std::cout << ">> Printing Faults List:" << std::endl;
        seulist.print();


        // Check
        debug::mac(dat, wt, psums_check, _ATOMIC);


        // DUT
        dut.reset();

        for(ember::time_t<unsigned> tick=0; tick<SIMTIME; tick++)
        {
            // Gen Fault Mask
            if(!seulist.empty()) {
                while(!seulist.empty() && ((seulist.getHead()).time() == tick)) {
                    ember::fault::fault_t<ember::fault::model, long unsigned> _fault = seulist.getHead();
                    std::cout << ">> SimTime = " << tick << ": \033[1;31minjecting fault " << _fault << "\033[0m" << std::endl;
                    _fault.location()->genFaultMask(_fault.fmodel());
                    seulist.pop();
                }
            }

            // Inject Faults
            for(size_t it=0; it<reglist.size(); it++) {
                ember::ISaboteur* reg = reglist[it];
                reg->applyAllFaults();
            }

            // Evaluate
            dut.update();

            // Check Results
            if(dut.psums.read() != psums_check) {
                std::cout << "-I: \033[1;31moutput = " << dut.psums.read() << "\033[0m (" << psums_check << " expected)" << std::endl;
            }
            else {
                std::cout << "-I: output = " << dut.psums.read() << std::endl;
            }
        }


        // Check Results
        if(dut.psums.read() != psums_check) {
            std::cout << "-E: [\033[1;31mtest " << testit << "\033[0m] Results mismatch: " << psums_check << " expected; \033[1;31m" << dut.psums.read() << "\033[0m obtained" << std::endl;
        }
        else {
            std::cout << ">> [test " << testit << "] Results match: " << dut.psums.read() << ", " << std::flush;
            if(dut.ready.read()) {
                std::cout << "outValid = " << dut.ready.read() << std::endl;
            }
            else {
                std::cout << "\033[1;31moutValid = " << dut.ready.read() << "\033[0m" << std::endl;
            }
        }
        std::cout << "===========================================================================================================" << std::endl;
    }

    std::cout << "-I: All test performed successfully" << std::endl;

    return 0;
}