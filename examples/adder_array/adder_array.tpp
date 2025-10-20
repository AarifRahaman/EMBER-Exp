#ifndef __ADDER_ARRAY_TPP__
#define __ADDER_ARRAY_TPP__

//
// === adder_array === //
//
template<typename T>
const char* debug::adder_array<T>::id() {
    return name.c_str();
}

template<typename T>
void debug::adder_array<T>::reset()
{
    outbuf->reset();
}

template<typename T>
std::vector<ember::ISaboteur*> debug::adder_array<T>::getSaboteurs()
{
    std::vector<ember::ISaboteur*> regs;
    regs.push_back(static_cast<ember::ISaboteur*>(outbuf));
    return regs;
}

template<typename T>
void debug::adder_array<T>::update()
{
    // Submodules update
    outbuf->update();
}

template<typename T>
void debug::adder_array<T>::eval()
{
    // ALUs
    for(size_t it=0; it<thpt; it++) {
        aux[it] = A.read()[it] + B.read()[it];
    }
    outbuf->din.write(aux);
}

template<typename T>
void debug::adder_array<T>::connect()
{
    // Self Binded
    A.bind();
    B.bind();
    outbuf->din.bind();
    outbuf->dout.bind();

    // Connections
    C.bind(outbuf->dout);
}

template<typename T>
void debug::adder_array<T>::connect(ember::port<T>* _A, ember::port<T>* _B)
{
    // Self Binded
    outbuf->din.bind();
    outbuf->dout.bind();

    // Connections
    A.bind(*_A);
    B.bind(*_B);
    C.bind(outbuf->dout);
}

template<typename T>
debug::adder_array<T>::adder_array(
    const size_t _thpt,
    const size_t _inBwA,
    const size_t _inBwB)
    :   thpt(_thpt),
        inBwA(_inBwA),
        inBwB(_inBwB),
        outBw(std::max(inBwA,inBwB)+1),
        name("adder_array"),
        aux(_thpt),
        A("A", _thpt, inBwA),
        B("B", _thpt, inBwB),
        C("C", _thpt, outBw)
{
    outbuf = new ember::saboteur::ff<ember::array<T> >("outbuf", thpt, outBw);
}

template<typename T>
debug::adder_array<T>::~adder_array()
{
    delete outbuf;
}

#endif