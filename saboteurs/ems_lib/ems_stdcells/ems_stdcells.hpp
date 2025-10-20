#ifndef __EMBER_SABOTEURS_STDCELLS_HPP__
#define __EMBER_SABOTEURS_STDCELLS_HPP__

#include "em_port.hpp"
#include "em_fault.hpp"

namespace ember {

namespace saboteur {

namespace priv {

class gate : public ember::ISaboteur, ember::IModule
{
protected:
    const std::string name;

    bool seuMask;
    bool sa0Mask;
    bool sa1Mask;

public:

    // Methods - IModule
    const char* id();
    void update() { seuMask = false; }
    void eval() { }
    std::vector<ISaboteur*> getSaboteurs();
    void reset() { }

    // Methods - ISaboteur
    const size_t locations(const fault::model& fModel) const;

    void genFaultMask(const fault::model& fModel);

    void clearFaultMask(const fault::model& fModel);
    void clearAllMasks();

    void applyFault(const fault::model& fModel) { }
    void applyAllFaults() { }

    // CTORS
    gate(const char* _id)       : name(_id) { clearAllMasks(); }
    gate(const std::string _id) : name(_id) { clearAllMasks(); }
    ~gate() { }
};

}

class buf: public priv::gate
{
public:
    // IO Port
    ember::inPort<bool>  A;
    ember::outPort<bool> Z;

    // Methods
    void eval();

    // CTORs
    buf(const char* _id);
    buf(const std::string _id);
    ~buf() { }
};

class inv: public priv::gate
{
public:
    // IO Port
    ember::inPort<bool>  A;
    ember::outPort<bool> Z;

    // Methods
    void eval();

    // CTORs
    inv(const char* _id);
    inv(const std::string _id);
    ~inv() { }
};

class and2: public priv::gate
{
public:
    // IO Ports
    ember::inPort<bool>  A;
    ember::inPort<bool>  B;
    ember::outPort<bool> Z;

    // Methods
    void eval();

    // CTORs
    and2(const char* _id);
    and2(const std::string _id);
    ~and2() { }
};

class and3: public priv::gate
{
public:
    // IO Ports
    ember::inPort<bool>  A;
    ember::inPort<bool>  B;
    ember::inPort<bool>  C;
    ember::outPort<bool> Z;

    // Methods
    void eval();

    // CTORs
    and3(const char* _id);
    and3(const std::string _id);
    ~and3() { }
};

class nand2: public priv::gate
{
public:
    // IO Ports
    ember::inPort<bool>  A;
    ember::inPort<bool>  B;
    ember::outPort<bool> Z;

    // Methods
    void eval();

    // CTORs
    nand2(const char* _id);
    nand2(const std::string _id);
    ~nand2() { }
};

class nand3: public priv::gate
{
public:
    // IO Ports
    ember::inPort<bool>  A;
    ember::inPort<bool>  B;
    ember::inPort<bool>  C;
    ember::outPort<bool> Z;

    // Methods
    void eval();

    // CTORs
    nand3(const char* _id);
    nand3(const std::string _id);
    ~nand3() { }
};

class or2: public priv::gate
{
public:
    // IO Ports
    ember::inPort<bool>  A;
    ember::inPort<bool>  B;
    ember::outPort<bool> Z;

    // Methods
    void eval();

    // CTORs
    or2(const char* _id);
    or2(const std::string _id);
    ~or2() { }
};

class or3: public priv::gate
{
public:
    // IO Ports
    ember::inPort<bool>  A;
    ember::inPort<bool>  B;
    ember::inPort<bool>  C;
    ember::outPort<bool> Z;

    // Methods
    void eval();

    // CTORs
    or3(const char* _id);
    or3(const std::string _id);
    ~or3() { }
};

class nor2: public priv::gate
{
public:
    // IO Ports
    ember::inPort<bool>  A;
    ember::inPort<bool>  B;
    ember::outPort<bool> Z;

    // Methods
    void eval();

    // CTORs
    nor2(const char* _id);
    nor2(const std::string _id);
    ~nor2() { }
};

class nor3: public priv::gate
{
public:
    // IO Ports
    ember::inPort<bool>  A;
    ember::inPort<bool>  B;
    ember::inPort<bool>  C;
    ember::outPort<bool> Z;

    // Methods
    void eval();

    // CTORs
    nor3(const char* _id);
    nor3(const std::string _id);
    ~nor3() { }
};

class xor2: public priv::gate
{
public:
    // IO Ports
    ember::inPort<bool>  A;
    ember::inPort<bool>  B;
    ember::outPort<bool> Z;

    // Methods
    void eval();

    // CTORs
    xor2(const char* _id);
    xor2(const std::string _id);
    ~xor2() { }
};

class xnor2: public priv::gate
{
public:
    // IO Ports
    ember::inPort<bool>  A;
    ember::inPort<bool>  B;
    ember::outPort<bool> Z;

    // Methods
    void eval();

    // CTORs
    xnor2(const char* _id);
    xnor2(const std::string _id);
    ~xnor2() { }
};

// template<typename T>
// class mux2: public priv::gate
// {
// public:
//     // IO Ports
//     ember::inPort<T>  A;
//     ember::inPort<T>  B;
//     ember::inPort<bool>  S;
//     ember::outPort<T> Z;

//     // Methods
//     void eval();

//     // CTORs
//     mux2(const char* _id);
//     ~mux2() { }
// };

}

}

#endif