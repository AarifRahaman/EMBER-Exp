#ifndef __ADDER_HPP__
#define __ADDER_HPP__

#include "ember.hpp"
#include "ember_saboteurs.hpp"

namespace debug {

template<typename T>
class adder : public ember::IModule {
private:
    const std::string name;
    const size_t inBwA;
    const size_t inBwB;
    const size_t outBw;

public:

    ember::inPort<T> A, B;
    ember::outPort<T> C;

    void connect();
    void connect(ember::port<T>* _A, ember::port<T>* _B);

    const char* id();
    void reset();
    void update();
    void eval();
    std::vector<ember::ISaboteur*> getSaboteurs();
    
    adder(const char* _id, const size_t bitwidth_a, const size_t bitwidth_b);
    // adder(const char* _id, const size_t bitwidth_a, const size_t bitwidth_b, ember::port<T>* _A, ember::port<T>* _B);
    virtual ~adder();
};

}

#include "adder.tpp"

#endif