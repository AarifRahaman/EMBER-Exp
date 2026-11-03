#pragma once


//
// == FAULT_T == //
//
template <class model_t>
ember::sim::Tick ember::fault::fault_t<model_t>::time() const {
    return _itime;
}

template <class model_t>
model_t ember::fault::fault_t<model_t>::fmodel() const {
    return _model;
}

template <class model_t>
ember::ISaboteurBase* ember::fault::fault_t<model_t>::location() const {
    return _location;
}

template <class model_t> std::string_view ember::fault::fault_t<model_t>::id() const {
    return _name;
}

template <class model_t> std::string ember::fault::fault_t<model_t>::repr() const {
    std::stringstream repr_str;
    repr_str << "(" << _location->id();
    repr_str << ", " << _name;
    repr_str << ", " << _itime << ")";
    return repr_str.str();
}

template <class model_t>
ember::fault::fault_t<model_t>&
ember::fault::fault_t<model_t>::operator=(ember::fault::fault_t<model_t> const& other) {
    if (this == &other) {
        return *this;
    }
    std::string aux(other.id());

    _itime = other.time();
    _model = other.fmodel();
    _name = aux;
    _location = other.location();

    return *this;
}

template <class model_t>
bool ember::fault::fault_t<model_t>::operator==(
    const ember::fault::fault_t<model_t>& other) const {
    return (_itime == other.time()) && (_model == other.fmodel());
}

template <class model_t>
bool ember::fault::fault_t<model_t>::operator<(
    const ember::fault::fault_t<model_t>& other) const {
    return (_itime < other.time()) && (_model == other.fmodel());
}

template <class model_t>
bool ember::fault::fault_t<model_t>::operator>(
    const ember::fault::fault_t<model_t>& other) const {
    return (_itime > other.time()) && (_model == other.fmodel());
}

template <class model_t>
bool ember::fault::fault_t<model_t>::operator<=(
    const ember::fault::fault_t<model_t>& other) const {
    return (_itime <= other.time()) && (_model == other.fmodel());
}

template <class model_t>
bool ember::fault::fault_t<model_t>::operator>=(
    const ember::fault::fault_t<model_t>& other) const {
    return (_itime >= other.time()) && (_model == other.fmodel());
}

template <class model_t>
ember::fault::fault_t<model_t>::fault_t(std::string_view model_str,const model_t fmodel,
                                           const ember::sim::Tick time, ember::ISaboteurBase* location)
       :    _name(model_str),
            _model(fmodel),
            _itime(time),
            _location(location){}