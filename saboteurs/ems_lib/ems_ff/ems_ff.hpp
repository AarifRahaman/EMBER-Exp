#pragma once

#include "emf_models.hpp"
#include "em_fault.hpp"
#include "em_imodule.hpp"
#include "em_isaboteur.hpp"
#include "em_random.hpp"
#include "em_types.hpp"

#include <deque>
#include <type_traits>
#include <vector>

namespace ember
{

namespace saboteur
{

namespace priv
{

// Single-data flip-flop (the array version was removed)
template <typename T>
requires (!is_array<T>::value)
class flipflop : public ember::ISaboteur<ember::fault::seu_t>,
                 public ember::ISaboteur<ember::fault::sa1_t>,
                 public ember::ISaboteur<ember::fault::sa0_t>,
                 public ember::IModule
  {

    protected:
    const std::string name;
    const size_t bw;
    size_t freeSaLocations;
    fault::mask_t* seuMask;
    fault::mask_t* sa0Mask;
    fault::mask_t* sa1Mask;
    std::deque<size_t> cellList;

    void resetCellList ();

    void genSeuMask ();
    void genSa1Mask ();
    void genSa0Mask ();
    void applySeuFault ();
    void applySa1Fault ();
    void applySa0Fault ();
    void clearSeuMask ();
    void clearSa1Mask ();
    void clearSa0Mask ();

  public:
    // IO Ports
    inPort<T> din;
    outPort<T> dout;

    // Methods - IModule
    const char* id ();
    void update ();
    std::vector<ISaboteurBase*> getSaboteurs ();
    void reset ();
    void eval () {}

    // Methods - ISaboteur
    const size_t locations (const std::type_identity<ember::fault::seu_t>) const override;
    const size_t locations (const std::type_identity<ember::fault::sa1_t>) const override;
    const size_t locations (const std::type_identity<ember::fault::sa0_t>) const override;

    void genFaultMask (const std::type_identity<ember::fault::seu_t>) override;
    void genFaultMask (const std::type_identity<ember::fault::sa1_t>) override;
    void genFaultMask (const std::type_identity<ember::fault::sa0_t>) override;

    void clearFaultMask (const std::type_identity<ember::fault::seu_t>) override;
    void clearFaultMask (const std::type_identity<ember::fault::sa1_t>) override;
    void clearFaultMask (const std::type_identity<ember::fault::sa0_t>) override;

    void applyFault (const std::type_identity<ember::fault::seu_t>) override;
    void applyFault (const std::type_identity<ember::fault::sa1_t>) override;
    void applyFault (const std::type_identity<ember::fault::sa0_t>) override;

    void clearAllMasks () override;
    void applyAllFaults () override;
    size_t faultModelCount () const override;

    // CTORS
    flipflop (const size_t& bitwidth);
    flipflop (const char* instanceId, const size_t& bitwidth);
    ~flipflop ();
};

} // namespace priv

// Helper Aliases
template <typename T> using flipflop = priv::flipflop<T>;

template <typename T> using ff = flipflop<T>;

} // namespace saboteur

} // namespace ember

#include "tpp/ems_ff.tpp"