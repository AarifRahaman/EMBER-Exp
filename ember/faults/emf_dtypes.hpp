#ifndef __EMBER_FAULT_DTYPE_HPP__
#define __EMBER_FAULT_DTYPE_HPP__

#include <string>
#include <sstream>

#include "em_time.hpp"
#include "em_isaboteur.hpp"

namespace ember {

namespace fault {

//
// == Parental Fault Class == //
//
template<class model_t, typename T=long unsigned>
class fault_t {
protected:
    std::string _name;
    model_t     _model;
    time_t<T>   _itime;
    ISaboteur* _location;

public:

    const time_t<T> time() const;
    const model_t fmodel() const;
    ISaboteur* location() const;
    const char* id() const;
    const char* repr() const;

    fault_t& operator=(const fault_t& other);

    bool operator==(const fault_t& other) const;
    bool operator<(const fault_t& other) const;
    bool operator>(const fault_t& other) const;
    bool operator<=(const fault_t& other) const;
    bool operator>=(const fault_t& other) const;

    fault_t(const model_t model, const char* model_str, const time_t<T> time, ISaboteur* location);
};


}

}

// Stream Out ember::fault::fault_t
template<class model_t, typename T>
std::ostream& operator<<(std::ostream& os, const ember::fault::fault_t<model_t, T>& other)
{
    os << "(" << other.location()->id();
    os << ", " << other.id();
    os << ", " << other.time() << ")" << std::flush;
    return os;
}


#include "tpp/emf_dtypes.tpp"

#endif