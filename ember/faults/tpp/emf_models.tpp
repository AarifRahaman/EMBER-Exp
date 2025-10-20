#ifndef __EMBER_FAULT_MODELS_TPP__
#define __EMBER_FAULT_MODELS_TPP__

//
// == SEU_T == //
//
template<typename T>
ember::fault::seu_t<T>& ember::fault::seu_t<T>::operator=(ember::fault::fault_t<ember::fault::model, T> const& other) {
    if(this == &other) { return *this; }

    static_assert(other.fmodel() == ember::fault::model::seu);
    _name = "seu";
    _model = ember::fault::model::seu;
    _itime = other.time();
    _location = other.location();

    return *this;
}

template <typename T>
ember::fault::seu_t<T>::seu_t(
    const ember::time_t<T> time,
    ember::ISaboteur* location
    )
    :   ember::fault::fault_t<ember::fault::model, T>::fault_t(ember::fault::model::seu, "seu", time, location)
{ }

template <typename T>
ember::fault::seu_t<T>::seu_t(
    const ember::fault::fault_t<ember::fault::model, T>& other
    )
    :   ember::fault::seu_t<T>::seu_t(other.time(), other.location())
{ }


//
// == SA0_T == //
//
template<typename T>
ember::fault::sa0_t<T>& ember::fault::sa0_t<T>::operator=(ember::fault::fault_t<ember::fault::model, T> const& other) {
    if(this == &other) { return *this; }

    static_assert(other.fmodel() == ember::fault::model::sa0);
    _name = "sa0";
    _model = ember::fault::model::sa0;
    _itime = other.time();
    _location = other.location();

    return *this;
}

template <typename T>
ember::fault::sa0_t<T>::sa0_t(
    const ember::time_t<T> time,
    ember::ISaboteur* location
    )
    :   ember::fault::fault_t<ember::fault::model, T>::fault_t(ember::fault::model::sa0, "seu", time, location)
{ }

template <typename T>
ember::fault::sa0_t<T>::sa0_t(
    const ember::fault::fault_t<ember::fault::model, T>& other
    )
    :   ember::fault::sa0_t<T>::sa0_t(other.time(), other.location())
{ }


//
// == SA1_T == //
//
template<typename T>
ember::fault::sa1_t<T>& ember::fault::sa1_t<T>::operator=(ember::fault::fault_t<ember::fault::model, T> const& other) {
    if(this == &other) { return *this; }

    static_assert(other.fmodel() == ember::fault::model::sa1);
    _name = "sa1";
    _model = ember::fault::model::sa1;
    _itime = other.time();
    _location = other.location();

    return *this;
}

template <typename T>
ember::fault::sa1_t<T>::sa1_t(
    const ember::time_t<T> time,
    ember::ISaboteur* location
    )
    :   ember::fault::fault_t<ember::fault::model, T>::fault_t(ember::fault::model::sa1, "sa1", time, location)
{ }

template <typename T>
ember::fault::sa1_t<T>::sa1_t(
    const ember::fault::fault_t<ember::fault::model, T>& other
    )
    :   ember::fault::sa1_t<T>::sa1_t(other.time(), other.location())
{ }


#endif