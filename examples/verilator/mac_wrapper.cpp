
#include "mac_wrapper.hpp"


const char* mac_wrapper::id()
{
    return name.c_str();
}

void mac_wrapper::update()
{
    int          psums_i;

    // Sets inputs
    verilated->VALID_IN = valid.read();

    ember::verilator::logic2verilated<ember::int8_t, IData, 8>(dat.read(), verilated->IN_A, atomic);
    ember::verilator::logic2verilated<ember::int8_t, IData, 8>(wt.read(),  verilated->IN_B, atomic);

    // Evaluate
    verilated->CLK = 1;
    verilated->eval();
    verilated->CLK = 0;
    verilated->eval();

    ready.write(verilated->VALID_OUT);
    ember::verilator::verilated2logic<IData, int, 18>(verilated->RES, psums_i);
    psums.write(psums_i);
}

std::vector<ember::ISaboteur*> mac_wrapper::getSaboteurs()
{
    std::vector<ember::ISaboteur*> components;
    components.push_back(static_cast<ember::ISaboteur*>(mul2add_reg));
    components.push_back(static_cast<ember::ISaboteur*>(tree_out_reg));
    components.push_back(static_cast<ember::ISaboteur*>(adder_tree_bus));
    return components;
}

void mac_wrapper::reset()
{
    verilated->CLK  = 0;
    verilated->NRST = 0;
    verilated->VALID_IN = 0;

    verilated->eval();

    verilated->NRST = 1;
}

void mac_wrapper::connect()
{
    valid.bind();
    ready.bind();
    wt.bind();
    dat.bind();
    psums.bind();
}

mac_wrapper::mac_wrapper()
    : ember::IModule(),
      name("top"),
      atomic(_ATOMIC),
      valid("valid"),
      ready("ready"),
      dat("dat", atomic),
      wt("wt", atomic),
      psums("psums")
{
    verilated      = new Vmac;

    mul2add_reg    = new ember::verilator::saboteur<QData, 64>("mul2add_reg", &verilated->mac->mul2add_wire);
    tree_out_reg   = new ember::verilator::saboteur<IData, 18>("tree_out_reg", &verilated->mac->tree_out_wire);
    adder_tree_bus = new ember::verilator::saboteur<WData, 144>("adder_tree_bus", verilated->mac->MAC_adder_tree->bus);
}

mac_wrapper::~mac_wrapper()
{
    verilated->final();
    delete    mul2add_reg;
    delete   tree_out_reg;
    delete adder_tree_bus;
    delete      verilated;
}
