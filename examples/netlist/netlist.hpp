#ifndef __NETLIST_HPP__
#define __NETLIST_HPP__

#include "ember.hpp"
#include "ember_saboteurs.hpp"

//* =================================
// This example contains a simple 
// gate level netlist with input
// and output registers.
// The path contains simple boolean
// gates of some binary inputs.
//* =================================

class netlist : public ember::IModule {
private:
    const std::string name;

    // Internal saboteurs
    ember::saboteur::ff<bool>    a_reg;
    ember::saboteur::ff<bool>    b_reg;
    ember::saboteur::ff<bool>    c_reg;
    ember::saboteur::ff<bool>    d_reg;
    ember::saboteur::ff<bool>    out_reg;
    ember::saboteur::and2        and2_0;
    ember::saboteur::inv         inv_0;
    ember::saboteur::or3         or3_0;


public:

    // IO Ports
    ember::inPort<bool>     a, b, c, d;
    ember::outPort<bool>    out;

    // ember Module Methods
    const char* id();
    void reset();
    void update();
    void eval();
    std::vector<ember::ISaboteur*> getSaboteurs();
    
    netlist(const char* _id);
    virtual ~netlist();
};

#include "netlist.tpp"

#endif