#ifndef __EMBER_FAULT_DTYPE_TPP__
#define __EMBER_FAULT_DTYPE_TPP__

//
// == FAULT_T == //
//
template<class model_t, typename T>
const ember::time_t<T> ember::fault::fault_t<model_t, T>::time() const {
    return _itime;
}

template<class model_t, typename T>
const model_t ember::fault::fault_t<model_t, T>::fmodel() const {
    return _model;
}

template<class model_t, typename T>
ember::ISaboteur* ember::fault::fault_t<model_t, T>::location() const {
    return _location;
}

template<class model_t, typename T>
const char* ember::fault::fault_t<model_t, T>::id() const {
    return _name.c_str();
}

template<class model_t, typename T>
const char* ember::fault::fault_t<model_t, T>::repr() const {
    std::stringstream repr_str;
    repr_str << "(" << _location->id();
    repr_str << ", " << _name.c_str();
    repr_str << ", " << _itime << ")";
    return repr_str.str().c_str();
}

template<class model_t, typename T>
ember::fault::fault_t<model_t, T>& ember::fault::fault_t<model_t, T>::operator=(ember::fault::fault_t<model_t, T> const& other) {
    if(this == &other) { return *this; }
    std::string aux(other.id());

    _itime = other.time();
    _model = other.fmodel();
    _name = aux;
    _location = other.location();

    return *this;
}

template<class model_t, typename T>
bool ember::fault::fault_t<model_t, T>::operator==(const ember::fault::fault_t<model_t, T>& other) const {
    return (_itime == other.time()) && (_model == other.fmodel());
}

template<class model_t, typename T>
bool ember::fault::fault_t<model_t, T>::operator<(const ember::fault::fault_t<model_t, T>& other) const {
    return (_itime < other.time()) && (_model == other.fmodel());
}

template<class model_t, typename T>
bool ember::fault::fault_t<model_t, T>::operator>(const ember::fault::fault_t<model_t, T>& other) const {
    return (_itime > other.time()) && (_model == other.fmodel());
}

template<class model_t, typename T>
bool ember::fault::fault_t<model_t, T>::operator<=(const ember::fault::fault_t<model_t, T>& other) const {
    return (_itime <= other.time()) && (_model == other.fmodel());
}

template<class model_t, typename T>
bool ember::fault::fault_t<model_t, T>::operator>=(const ember::fault::fault_t<model_t, T>& other) const {
    return (_itime >= other.time()) && (_model == other.fmodel());
}

template<class model_t, typename T>
ember::fault::fault_t<model_t, T>::fault_t(
    const model_t fmodel,
    const char* model_str,
    const ember::time_t<T> time,
    ember::ISaboteur* location
    )
    :   _model(fmodel),
        _itime(time),
        _location(location),
        _name(model_str)
{ }

#endif