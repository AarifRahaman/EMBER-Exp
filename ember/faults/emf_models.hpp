#ifndef __EMBER_FAULT_MODELS_HPP__
#define __EMBER_FAULT_MODELS_HPP__

#include "em_time.hpp"
#include "em_isaboteur.hpp"

namespace ember {

namespace fault {

//
// == SEU Class == //
//
template<typename T=long unsigned>
class seu_t : public fault_t<model, T> {
protected:
    std::string _name;
    model _model;
    time_t<T> _itime;
    ISaboteur* _location;

public:

    seu_t& operator=(const fault_t<model, T>& other);

    seu_t(const time_t<T> fault_time, ISaboteur* location);
    seu_t(const fault_t<model, T>& other);
};



//
// == Stuck-At Classes == //
//
template<typename T=long unsigned>
class sa0_t : public fault_t<model, T> {
protected:
    std::string _name;
    model _model;
    time_t<T> _itime;
    ISaboteur* _location;

public:

    sa0_t& operator=(const fault_t<model, T>& other);

    sa0_t(const time_t<T> fault_time, ISaboteur* location);
    sa0_t(const fault_t<model, T>& other);
};

template<typename T=long unsigned>
class sa1_t : public fault_t<model, T> {
protected:
    std::string _name;
    model _model;
    time_t<T> _itime;
    ISaboteur* _location;

public:

    sa1_t& operator=(const fault_t<model, T>& other);

    sa1_t(const time_t<T> fault_time, ISaboteur* location);
    sa1_t(const fault_t<model, T>& other);
};

}

}

#include "tpp/emf_models.tpp"

#endif