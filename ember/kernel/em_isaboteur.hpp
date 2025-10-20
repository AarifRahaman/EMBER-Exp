#ifndef __EMBER_ISABOTEUR_HPP__
#define __EMBER_ISABOTEUR_HPP__

#include <cstdint>
#include <cstddef>

#include "em_iobject.hpp"
#include "emf_baseline_dtypes.hpp"

namespace ember {

// Component Interface
class ISaboteur : public IObject
{
protected:
    ISaboteur() { }

public:
    virtual const size_t locations(const ember::fault::model& fModel) const = 0;

    virtual void genFaultMask(const ember::fault::model& fModel) = 0;

    virtual void clearFaultMask(const ember::fault::model& fModel) = 0;
    virtual void clearAllMasks() = 0;

    virtual void applyFault(const ember::fault::model& fModel) = 0;
    virtual void applyAllFaults() = 0;
};

}

#endif