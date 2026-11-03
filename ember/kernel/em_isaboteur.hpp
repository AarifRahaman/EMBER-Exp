#pragma once

#include "em_isaboteurbase.hpp"
#include "emf_baseline_dtypes.hpp"
#include "FM_contracts.hpp"

#include <cstddef>



namespace ember {

// Component Interface

template<FaultModel FM>
class ISaboteur : public virtual ISaboteurBase {
  protected:
    ISaboteur() {}

  public:
    virtual const size_t locations(const std::type_identity<FM>) const = 0;

    virtual void genFaultMask(const std::type_identity<FM>) = 0;

    virtual void clearFaultMask(const std::type_identity<FM>) = 0;


    virtual void applyFault(const std::type_identity<FM>) = 0;


};

} // namespace ember
