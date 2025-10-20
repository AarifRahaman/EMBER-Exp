#ifndef __EMBER_SABOTEURS_FLIPFLOP_HPP__
#define __EMBER_SABOTEURS_FLIPFLOP_HPP__

#include <deque>

#include "em_random.hpp"
#include "em_isaboteur.hpp"
#include "em_imodule.hpp"
#include "em_fault.hpp"
#include "em_types.hpp"

namespace ember {

namespace saboteur {

namespace priv {

// General Declaration
template<typename T, bool is_array>
class flipflop : public ember::ISaboteur, ember::IModule
{
    // Just Declaration
};


// is_array = True Declaration
template<typename T>
class flipflop<T, true> : public ember::ISaboteur, ember::IModule
{
protected:
    const std::string name;
    const size_t size;
    const size_t singleBw;
    const size_t totalBw;
    size_t freeSaLocations;
    fault::mask_t* seuMask;
    fault::mask_t* sa0Mask;
    fault::mask_t* sa1Mask;
    T aux;
    std::deque<size_t> cellList;

    void resetCellList();

    void genSeuMask();
    void genSa1Mask();
    void genSa0Mask();
    void applySeuFault();
    void applySa1Fault();
    void applySa0Fault();
    void clearSeuMask();
    void clearSa1Mask();
    void clearSa0Mask();

public:

    // IO Ports
    inPort<T>    din;
    outPort<T>  dout;

    // Methods - IModule
    const char* id();
    void update();
    std::vector<ISaboteur*> getSaboteurs();
    void reset();
    void eval() { }

    // Methods - ISaboteur
    const size_t locations(const fault::model& fModel) const;

    void genFaultMask(const fault::model& fModel);

    void clearFaultMask(const fault::model& fModel);
    void clearAllMasks();

    void applyFault(const fault::model& fModel);
    void applyAllFaults();

    // CTORS
    flipflop(const size_t& bitwidth);
    flipflop(const char* instanceId, const size_t& bitwidth);
    flipflop(const char* instanceId, const size_t& len, const size_t& bitwidth);
    ~flipflop();
};



// is_array = False Declaration
template<typename T>
class flipflop<T, false> : public ember::ISaboteur, ember::IModule
{
protected:
    const std::string name;
    const size_t bw;
    size_t freeSaLocations;
    fault::mask_t* seuMask;
    fault::mask_t* sa0Mask;
    fault::mask_t* sa1Mask;
    std::deque<size_t> cellList;

    void resetCellList();

    void genSeuMask();
    void genSa1Mask();
    void genSa0Mask();
    void applySeuFault();
    void applySa1Fault();
    void applySa0Fault();
    void clearSeuMask();
    void clearSa1Mask();
    void clearSa0Mask();

public:

    // IO Ports
    inPort<T>    din;
    outPort<T>  dout;

    // Methods - IModule
    const char* id();
    void update();
    void eval() { }
    std::vector<ISaboteur*> getSaboteurs();
    void reset();

    // Methods - ISaboteur
    const size_t locations(const fault::model& fModel) const;

    void genFaultMask(const fault::model& fModel);

    void clearFaultMask(const fault::model& fModel);
    void clearAllMasks();

    void applyFault(const fault::model& fModel);
    void applyAllFaults();

    // CTORS
    flipflop(const size_t& bitwidth);
    flipflop(const char* instanceId, const size_t& bitwidth);
    ~flipflop();
};

}


// Helper Aliases
template<typename T>
using flipflop = priv::flipflop<T, ember::is_array<T>::value >;

template<typename T>
using ff = flipflop<T>;

} // namespace saboteur
    
} // namespace ember

#include "tpp/ems_ff.tpp"
#include "tpp/ems_ff_array.tpp"

#endif