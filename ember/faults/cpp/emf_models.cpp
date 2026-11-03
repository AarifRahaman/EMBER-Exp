#include "emf_models.hpp"
#include <cassert>

//
// == SEU_T == //
//

ember::fault::seu_t&
ember::fault::seu_t::operator=(ember::fault::fault_t<ember::fault::model> const& other) {
    if (this == &other) {
        return *this;
    }

    assert(other.fmodel() == ember::fault::model::seu);
    _name = "seu";
    _model = ember::fault::model::seu;
    _itime = other.time();
    _location = other.location();

    return *this;
}


ember::fault::seu_t::seu_t(const ember::sim::Tick time, ember::ISaboteurBase* location)
    : ember::fault::fault_t<ember::fault::model>::fault_t(ember::fault::model::seu, "seu", time,
                                                             location) {}


ember::fault::seu_t::seu_t(const ember::fault::fault_t<ember::fault::model>& other)
    : ember::fault::seu_t::seu_t(other.time(), other.location()) {}

//
// == SA0_T == //
//

ember::fault::sa0_t&
ember::fault::sa0_t::operator=(ember::fault::fault_t<ember::fault::model> const& other) {
    if (this == &other) {
        return *this;
    }

    assert(other.fmodel() == ember::fault::model::sa0);
    _name = "sa0";
    _model = ember::fault::model::sa0;
    _itime = other.time();
    _location = other.location();

    return *this;
}


ember::fault::sa0_t::sa0_t(const ember::sim::Tick time, ember::ISaboteurBase* location)
    : ember::fault::fault_t<ember::fault::model>::fault_t(ember::fault::model::sa0, "sa0", time,
                                                             location) {}


ember::fault::sa0_t::sa0_t(const ember::fault::fault_t<ember::fault::model>& other)
    : ember::fault::sa0_t::sa0_t(other.time(), other.location()) {}

//
// == SA1_T == //
//

ember::fault::sa1_t&
ember::fault::sa1_t::operator=(ember::fault::fault_t<ember::fault::model> const& other) {
    if (this == &other) {
        return *this;
    }

    assert(other.fmodel() == ember::fault::model::sa1);
    _name = "sa1";
    _model = ember::fault::model::sa1;
    _itime = other.time();
    _location = other.location();

    return *this;
}


ember::fault::sa1_t::sa1_t(const ember::sim::Tick time, ember::ISaboteurBase* location)
    : ember::fault::fault_t<ember::fault::model>::fault_t(ember::fault::model::sa1, "sa1", time,
                                                             location) {}


ember::fault::sa1_t::sa1_t(const ember::fault::fault_t<ember::fault::model>& other)
    : ember::fault::sa1_t::sa1_t(other.time(), other.location()) {}
