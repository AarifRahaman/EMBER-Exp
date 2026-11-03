#pragma once

#include "em_isaboteur.hpp"
#include "em_tick.hpp"

#include <sstream>
#include <string>
#include <string_view>

namespace ember {

namespace fault {

//
// == Parental Fault Class == //
//
template <class model_t>
class fault_t {
  protected:
    std::string _name;
    model_t _model;
    sim::Tick _itime;
    ISaboteurBase* _location;

  public:
    sim::Tick time() const;
    model_t fmodel() const;
    ISaboteurBase* location() const;
    std::string_view id() const;
    std::string repr() const;

    fault_t& operator=(const fault_t& other);

    bool operator==(const fault_t& other) const;
    bool operator<(const fault_t& other) const;
    bool operator>(const fault_t& other) const;
    bool operator<=(const fault_t& other) const;
    bool operator>=(const fault_t& other) const;

    fault_t(std::string_view model_str, const model_t model, const sim::Tick time, ISaboteurBase* location);
};

} // namespace fault

} // namespace ember

// Stream Out ember::fault::fault_t
template <class model_t>
std::ostream& operator<<(std::ostream& os, const ember::fault::fault_t<model_t>& other) {
    os << "(" << other.location()->id();
    os << ", " << other.id();
    os << ", " << other.time() << ")" << std::flush;
    return os;
}

#include "tpp/emf_dtypes.tpp"