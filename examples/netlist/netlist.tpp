#ifndef __NETLIST_TPP__
#define __NETLIST_TPP__

// Simulator-specific Method
const char* netlist::id()
{
    return name.c_str();
}

// Registers Reset 
void netlist::reset()
{
    // Registers
    a_reg.reset();
    b_reg.reset();
    c_reg.reset();
    d_reg.reset();
    out_reg.reset();
    // Gates
    or3_0.reset();
    inv_0.reset();
    and2_0.reset();
}

// Returns Saboteurs
std::vector<ember::ISaboteur*> netlist::getSaboteurs()
{
    std::vector<ember::ISaboteur*> muts;

    // Registers
    muts.push_back(dynamic_cast<ember::ISaboteur*>(&a_reg));
    muts.push_back(dynamic_cast<ember::ISaboteur*>(&b_reg));
    muts.push_back(dynamic_cast<ember::ISaboteur*>(&c_reg));
    muts.push_back(dynamic_cast<ember::ISaboteur*>(&d_reg));
    muts.push_back(dynamic_cast<ember::ISaboteur*>(&out_reg));

    // Signals
    muts.push_back(dynamic_cast<ember::ISaboteur*>(&and2_0));
    muts.push_back(dynamic_cast<ember::ISaboteur*>(&inv_0));
    muts.push_back(dynamic_cast<ember::ISaboteur*>(&or3_0));

    return muts;
}

// Clock Edge Method
void netlist::update()
{
    // Regs
    a_reg.update();
    b_reg.update();
    c_reg.update();
    d_reg.update();
    out_reg.update();
    // Gates
    and2_0.update();
    inv_0.update();
    or3_0.update();
}

// Combinational Logic
void netlist::eval()
{
    // Regs
    a_reg.eval();
    b_reg.eval();
    c_reg.eval();
    d_reg.eval();
    out_reg.eval();
    // Gates
    and2_0.eval();
    inv_0.eval();
    or3_0.eval();
}

// Constructor
netlist::netlist(const char* _id)
    :   
    name(_id),
    // Signal Variable(Signal ID, bitwidth)
    a_reg("a_reg",     1),
    b_reg("b_reg",     1),
    c_reg("c_reg",     1),
    d_reg("d_reg",     1),
    out_reg("out_reg", 1),
    and2_0("and2_0"),
    or3_0("or3_0"),
    inv_0("inv_0"),
    a("a"),
    b("b"),
    c("c"),
    d("d"),
    out("out")
{
    // IO Ports
    a.bind();
    b.bind();
    c.bind();
    d.bind();
    out.bind();

    // In Reg
    a_reg.din.bind(a);
    b_reg.din.bind(b);
    c_reg.din.bind(c);
    d_reg.din.bind(d);

    // AND2
    and2_0.A.bind(a_reg.dout);
    and2_0.B.bind(b_reg.dout);

    // INV
    inv_0.A.bind(c_reg.dout);

    // OR3
    or3_0.A.bind(inv_0.Z);
    or3_0.B.bind(and2_0.Z);
    or3_0.C.bind(d_reg.dout);    
    or3_0.Z.bind(out_reg.din);

    // Out Reg
    out_reg.dout.bind(out);
}

netlist::~netlist()
{ }

#endif