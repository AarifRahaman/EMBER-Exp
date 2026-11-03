#include "em_isaboteur.hpp"
#include "em_tick.hpp"
#include "emf_dtypes.hpp"
#include "emf_models.hpp"

#include <algorithm>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>

namespace ember {

namespace fault {

//
// == Fault List == //
//
template <ember::fault::model fmodel, typename T = long unsigned> class list {
  protected:
    std::deque<fault_t<ember::fault::model>> _fqueue;

  public:
    void insert(fault_t<ember::fault::model>& fault);
    void pop();
    fault_t<ember::fault::model>& getHead();
    size_t size() const;
    bool empty() const;
    size_t clear();
    void print() const;

    list();
};

} // namespace fault

} // namespace ember

#include "tpp/emf_list.tpp"
