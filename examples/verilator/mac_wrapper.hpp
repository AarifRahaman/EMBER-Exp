#ifndef __MAC_WRAPPER_HPP__
#define __MAC_WRAPPER_HPP__

#include <vector>
#include <string>

#include "ember.hpp"
#include "ember_verilator.hpp"

#include "verilated.h"
#include "Vmac.h"
#include "Vmac_mac.h"
#include "Vmac_VLIB_adder_tree__A4_B10.h"

#define _ATOMIC  4


class mac_wrapper : public ember::IModule {
protected:
    const size_t atomic;
    const std::string name;

    // saboteurs
    ember::verilator::saboteur<QData, 64>* mul2add_reg;
    ember::verilator::saboteur<IData, 18>* tree_out_reg;
    ember::verilator::saboteur<WData, 144>* adder_tree_bus;

public:

    // Internal Verilated DUT
    Vmac* verilated;

    // Ports
    ember::inPort<bool>                       valid;
    ember::outPort<bool>                      ready;
    ember::inPort<ember::array<ember::int8_t> > dat;
    ember::inPort<ember::array<ember::int8_t> > wt;
    ember::outPort<int>                       psums;

    void connect();

    // Core Routines
    const char* id();
    void update();
    void eval() { }
    std::vector<ember::ISaboteur*> getSaboteurs();
    void reset();

    mac_wrapper();
    ~mac_wrapper();
};

#endif