#ifndef __EMBER_FAULT_LIST_TPP__
#define __EMBER_FAULT_LIST_TPP__

//
// == FAULT LIST == //
//
template<ember::fault::model fmodel, typename T>
void ember::fault::list<fmodel, T>::insert(ember::fault::fault_t<ember::fault::model, T>& other) {
    if(_fqueue.empty()) {
        _fqueue.push_back(other);
    }
    else {
    auto pos = std::upper_bound(_fqueue.begin(), _fqueue.end(), other);
    _fqueue.insert(pos, other);
    }
}

template<ember::fault::model fmodel, typename T>
void ember::fault::list<fmodel, T>::pop() {
    _fqueue.pop_front();
}

template<ember::fault::model fmodel, typename T>
ember::fault::fault_t<ember::fault::model, T>& ember::fault::list<fmodel, T>::getHead() {
    return _fqueue.front();
}

template<ember::fault::model fmodel, typename T>
size_t ember::fault::list<fmodel, T>::size() const {
    return _fqueue.size();
}

template<ember::fault::model fmodel, typename T>
bool ember::fault::list<fmodel, T>::empty() const {
    return _fqueue.empty();
}

template<ember::fault::model fmodel, typename T>
size_t ember::fault::list<fmodel, T>::clear() {
    size_t len = _fqueue.size();
    _fqueue.clear();
    return len;
}

template<ember::fault::model fmodel, typename T>
void ember::fault::list<fmodel, T>::print() const {
    
    for(size_t it=0; it<_fqueue.size(); it++) {
        std::cout << _fqueue[it] << std::endl;
    }

    if(_fqueue.size() == 0) {
        std::cout << "<empty>" << std::endl;
    }
}

template<ember::fault::model fmodel, typename T>
ember::fault::list<fmodel, T>::list()
{
    clear();
}

#endif