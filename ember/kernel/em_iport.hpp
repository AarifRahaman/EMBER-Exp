#ifndef __EMBER_IPORT_HPP__
#define __EMBER_IPORT_HPP__

#include "em_iobject.hpp"

namespace ember
{

template<typename T>
class IPort : public IObject
{
protected:
    IPort() { }
    ~IPort() { }

public:

    virtual void bind(IPort<T>& other) = 0;
    virtual void write(const T& other) = 0;
    virtual const T& read() const = 0;
};

} // namespace ember


#endif