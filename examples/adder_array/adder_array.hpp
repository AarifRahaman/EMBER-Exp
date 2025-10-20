#ifndef __ADDER_ARRAY_HPP__
#define __ADDER_ARRAY_HPP__

#include "ember.hpp"
#include "ember_saboteurs.hpp"

namespace debug {

template<typename T>
class adder_array : public ember::IModule
{
protected:
    const size_t thpt;
    const size_t inBwA;
    const size_t inBwB;
    const size_t outBw;
    const std::string name;
    ember::array<T> aux;

    // Internal Register
    ember::saboteur::ff<ember::array<T> >* outbuf;

public:

    // IOs
    ember::inPort<ember::array<T> > A, B;
    ember::outPort<ember::array<T> > C;

    void connect();
    void connect(ember::port<T>* _A, ember::port<T>* _B);

    const char* id();
    void reset();
    void update();
    void eval();
    std::vector<ember::ISaboteur*> getSaboteurs();
    
    adder_array(const size_t _thpt, const size_t _inBwA, const size_t _inBwB);
    // adder_array(const size_t _thpt, const size_t _inBwA, const size_t _inBwB, T* _A, T* _B);
    virtual ~adder_array();
};

}

#include "adder_array.tpp"

#endif