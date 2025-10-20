#ifndef __EMBER_IOBJECT_HPP__
#define __EMBER_IOBJECT_HPP__

namespace ember
{

class IObject {
public:

    virtual const char* id() = 0;

};
    
} // namespace ember

#endif