#pragma once

#include "em_iobject.hpp"
#include "em_isaboteurbase.hpp"
#include "em_concepts.hpp"

#include <vector>
#include <memory>

namespace ember {

// Primitive Unit Interface
class IModule : public IObject {
  protected:
    IModule() {}

  public:
    // Core Routines
    virtual void update () = 0;
    virtual void eval () = 0;
    virtual std::vector<ISaboteurBase*> getSaboteurs () = 0;
    virtual void reset () = 0;
};



} // namespace ember
