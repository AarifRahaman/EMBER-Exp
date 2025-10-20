#ifndef __EMBER_FAULT_LIST_HPP__
#define __EMBER_FAULT_LIST_HPP__

#include <deque>
#include <algorithm>
#include <sstream>
#include <string>
#include <iostream>

#include "em_time.hpp"
#include "em_isaboteur.hpp"
#include "emf_dtypes.hpp"
#include "emf_models.hpp"

namespace ember {

namespace fault {


//
// == Fault List == //
//
template<ember::fault::model fmodel, typename T=long unsigned>
class list {
protected:
    std::deque<fault_t<ember::fault::model, T> > _fqueue;

public:

    void insert(fault_t<ember::fault::model, T>& fault);
    void pop();
    fault_t<ember::fault::model, T>& getHead();
    size_t size() const;
    bool empty() const;
    size_t clear();
    void print() const;

    list();
};

}

}

#include "tpp/emf_list.tpp"

#endif