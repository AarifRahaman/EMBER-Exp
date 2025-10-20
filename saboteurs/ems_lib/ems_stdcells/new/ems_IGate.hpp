#ifndef __EMBER_SABOTEURS_IGATE_HPP__
#define __EMBER_SABOTEURS_IGATE_HPP__

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

}

}

#endif