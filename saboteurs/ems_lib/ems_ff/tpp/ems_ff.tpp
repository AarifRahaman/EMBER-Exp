#ifndef __EMBER_SABOTEURS_FLIPFLOP_TPP__
#define __EMBER_SABOTEURS_FLIPFLOP_TPP__

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::resetCellList()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    for(size_t it=0; it<bw; it++) { cellList.push_back(it); }
    std::shuffle(cellList.begin(), cellList.end(), gen);
    freeSaLocations = bw;
}


template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::genSeuMask()
{
    size_t pos = ember::math::random::uniform<>((size_t) 0, (size_t) bw);
    *seuMask |= 0x1 << pos;

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-I(" << __func__ << "): Generated SeuMask " << *seuMask << std::endl;
#endif
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::genSa1Mask()
{
    if(!cellList.empty())
    {
        assert(freeSaLocations > 0);
        size_t fbit = cellList[0];

        *sa1Mask |= 0x1 << fbit;

        freeSaLocations--;
        cellList.pop_front();

#if defined(DEBUG) || defined(VERBOSE)
        assert(fbit < bw);

        std::cout << "-I(" << __func__ << "): Generated Sa1Mask " << *sa1Mask << std::endl;
#endif
    }
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::genSa0Mask()
{
    if(!cellList.empty())
    {
        assert(freeSaLocations > 0);
        size_t fbit = cellList[0];

        *sa0Mask &= ~(0x1 << fbit);

        freeSaLocations--;
        cellList.pop_front();

#if defined(DEBUG) || defined(VERBOSE)
        assert(fbit < bw);

        std::cout << "-I(" << __func__ << "): Generated Sa0Mask " << *sa0Mask << std::endl;
#endif
    }
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::applySeuFault()
{
    dout.write(dout.read() ^ *seuMask);

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-I(" << __func__ << "): Injecting SeuMask " << *seuMask << std::endl;
#endif

    clearSeuMask();
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::applySa1Fault()
{
    dout.write(dout.read() | *sa1Mask);

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-I(" << __func__ << "): Injecting Sa1Mask " << *sa1Mask << std::endl;
#endif
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::applySa0Fault()
{
    dout.write(dout.read() & *sa0Mask);

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-I(" << __func__ << "): Injecting Sa0Mask " << *sa0Mask << std::endl;
#endif
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::clearSeuMask()
{
    *seuMask = 0;
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::clearSa1Mask()
{
    *sa1Mask = 0;
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::clearSa0Mask()
{
    *sa0Mask = ~0;
}


template<typename T> 
const char* ember::saboteur::priv::flipflop<T, false>::id()
{
    return name.c_str();
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::reset()
{
    dout.write(0);
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::update()
{
    dout.write(din.read());
}

template<typename T> 
std::vector<ember::ISaboteur*> ember::saboteur::priv::flipflop<T, false>::getSaboteurs()
{
    std::vector<ember::ISaboteur*> aux;
    aux.push_back(this);
    return aux;
}

template<typename T> 
const size_t ember::saboteur::priv::flipflop<T, false>::locations(const ember::fault::model& fModel) const
{
    switch(fModel)
    {
        case ember::fault::model::seu:
        {
            return bw;
            break;
        }
        case ember::fault::model::sa0:
        {
            return freeSaLocations;
            break;
        }
        case ember::fault::model::sa1:
        {
            return freeSaLocations;
            break;
        }
        default:
        {
            throw std::invalid_argument("fModel is not supported");
        }
    }
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::genFaultMask(const ember::fault::model& fModel)
{
    switch(fModel)
    {
        case ember::fault::model::seu:
        {
            genSeuMask();
            break;
        }
        case ember::fault::model::sa0:
        {
            genSa0Mask();
            break;
        }
        case ember::fault::model::sa1:
        {
            genSa1Mask();
            break;
        }
        default:
        {
            throw std::invalid_argument("fModel is not supported");
        }
    }
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::clearFaultMask(const ember::fault::model& fModel)
{
    switch(fModel)
    {
        case ember::fault::model::seu:
        {
            clearSeuMask();
            break;
        }
        case ember::fault::model::sa0:
        {
            clearSa0Mask();
            break;
        }
        case ember::fault::model::sa1:
        {
            clearSa1Mask();
            break;
        }
        default:
        {
            throw std::invalid_argument("fModel is not supported");
        }
    }
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::clearAllMasks()
{
    clearSeuMask();
    clearSa0Mask();
    clearSa1Mask();
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::applyFault(const ember::fault::model& fModel)
{
    switch(fModel)
    {
        case ember::fault::model::seu:
        {
            applySeuFault();
            break;
        }
        case ember::fault::model::sa0:
        {
            applySa0Fault();
            break;
        }
        case ember::fault::model::sa1:
        {
            applySa1Fault();
            break;
        }
        default:
        {
            throw std::invalid_argument("fModel is not supported");
        }
    }
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, false>::applyAllFaults()
{
    applySeuFault();
    applySa0Fault();
    applySa1Fault();
}


template<typename T> 
ember::saboteur::priv::flipflop<T, false>::flipflop(const size_t& bitwidth)
    :   bw(bitwidth),
        name("reg"),
        din("din"),
        dout("dout")
{
    // dout.bind();

    seuMask = new ember::fault::mask_t;
    sa0Mask = new ember::fault::mask_t;
    sa1Mask = new ember::fault::mask_t;

    clearSeuMask();
    clearSa0Mask();
    clearSa1Mask();

    resetCellList();
}

template<typename T> 
ember::saboteur::priv::flipflop<T, false>::flipflop(const char* instanceId, const size_t& bitwidth)
    :   bw(bitwidth),
        name(instanceId),
        din("din"),
        dout("dout")
{
    // dout.bind();

    seuMask = new ember::fault::mask_t;
    sa0Mask = new ember::fault::mask_t;
    sa1Mask = new ember::fault::mask_t;

    clearSeuMask();
    clearSa0Mask();
    clearSa1Mask();

    resetCellList();
}

template<typename T> 
ember::saboteur::priv::flipflop<T, false>::~flipflop()
{
    delete seuMask;
    delete sa0Mask;
    delete sa1Mask;
}


#endif
