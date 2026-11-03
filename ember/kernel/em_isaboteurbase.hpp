#pragma once

#include "em_iobject.hpp"
#include <cstddef>


namespace ember {

class ISaboteurBase : public IObject {

protected:
    ISaboteurBase() {}

public:
    virtual void clearAllMasks() = 0;
    virtual void applyAllFaults() = 0;
    virtual std::size_t faultModelCount() const = 0;
};
}

// namespace ember