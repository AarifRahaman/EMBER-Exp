#ifndef __ADDER_WREG_TPP__
#define __ADDER_WREG_TPP__

//
// === adder_wreg === //
//

// PUBLIC
template<typename T>
const char* debug::adder_wreg<T>::id()
{
    return name.c_str();
}

template<typename T>
void debug::adder_wreg<T>::reset()
{
    outreg.reset();
}

template<typename T>
std::vector<ember::ISaboteur*> debug::adder_wreg<T>::getSaboteurs()
{
    std::vector<ember::ISaboteur*> regs;
    regs.push_back(dynamic_cast<ember::ISaboteur*>(&outreg));
    return regs;
}

template<typename T>
void debug::adder_wreg<T>::update()
{
    // Update Phase
    outreg.update();
    comb_add->update();
}

template<typename T>
void debug::adder_wreg<T>::eval()
{
    // Evaluation Phase
    outreg.eval();
    comb_add->eval();
}

template<typename T>
void debug::adder_wreg<T>::connect()
{
    // Self Binded
    A.bind();
    B.bind();
    outreg.dout.bind();
    // Connections
    comb_add->connect(&A, &B);
    outreg.din.bind(comb_add->C);
    C.bind(outreg.dout);
}

template<typename T>
void debug::adder_wreg<T>::connect(ember::port<T>* _A, ember::port<T>* _B)
{
    // Self Binded
    outreg.dout.bind();

    // Connections
    A.bind(*_A);
    B.bind(*_B);
    comb_add->connect(A, B);
    outreg.din.bind(comb_add->C);
    C.bind(outreg.dout);
}


template<typename T>
debug::adder_wreg<T>::adder_wreg(const char* _id, const size_t bitwidth_a, const size_t bitwidth_b)
    :   inBwA(bitwidth_a),
        inBwB(bitwidth_b),
        outBw(std::max(inBwA,inBwB)+1),
        name(_id),
        A("A"),
        B("B"),
        C("C"),
        outreg("outreg", outBw)
{
    comb_add = new debug::adder<T>("U", inBwA, inBwB);
}

template<typename T>
debug::adder_wreg<T>::~adder_wreg()
{
    delete comb_add;
}

#endif