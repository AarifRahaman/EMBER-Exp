#ifndef __EMBER_IMODULE_HPP__
#define __EMBER_IMODULE_HPP__

#include "em_iobject.hpp"
#include "em_isaboteur.hpp"

namespace ember {

// Primitive Unit Interface
class IModule : public IObject
{
protected:
    IModule() { }

public:

    // Core Routines
    virtual void update() = 0;
    virtual void eval() = 0;
    virtual std::vector<ISaboteur*> getSaboteurs() = 0;
    virtual void reset() = 0;
};

}

#endif
