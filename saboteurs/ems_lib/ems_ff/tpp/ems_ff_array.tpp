#ifndef __EMBER_SABOTEURS_FLIPFLOP_ARRAY_TPP__
#define __EMBER_SABOTEURS_FLIPFLOP_ARRAY_TPP__

template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::resetCellList()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    for(size_t it=0; it<totalBw; it++) { cellList.push_back(it); }
    std::shuffle(cellList.begin(), cellList.end(), gen);
    freeSaLocations = totalBw;
}


template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::genSeuMask()
{
    size_t it = ember::math::random::uniform<>((size_t)  0, (size_t) size);
    size_t pos = ember::math::random::uniform<>((size_t) 0, (size_t) singleBw);

    seuMask[it] |= 0x1 << pos;

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-I(" << __func__ << "): Generated SeuMask" << std::flush;
    for(size_t it=0; it<size; it++) {
        std::cout << " " << seuMask[it] << std::flush;
    }
    std::cout << std::endl;
#endif
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::genSa1Mask()
{
    if(!cellList.empty())
    {
        assert(freeSaLocations > 0);
        size_t fbit = cellList[0];
        size_t dstride = singleBw;
        size_t fword = fbit / dstride;
        size_t fpos  = fbit % dstride;

        sa1Mask[fword] |= 0x1 << fpos;

        freeSaLocations--;
        cellList.pop_front();

#if defined(DEBUG) || defined(VERBOSE)
        assert(fbit < totalBw);
        assert(fword < size);
        assert(fpos < dstride);

        std::cout << "-I(" << __func__ << "): Generated Sa0Mask " << std::flush;
        for(size_t it=0; it<size; it++) {
            std::cout << " " << sa1Mask[it];
        }
        std::cout << std::endl;
#endif
    } 
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::genSa0Mask()
{
    if(!cellList.empty())
    {
        assert(freeSaLocations > 0);
        size_t fbit = cellList[0];
        size_t dstride = singleBw;
        size_t fword = fbit / dstride;
        size_t fpos  = fbit % dstride;

        sa0Mask[fword] &= ~(0x1 << fpos);

        freeSaLocations--;
        cellList.pop_front();

#if defined(DEBUG) || defined(VERBOSE)
        assert(fbit < totalBw);
        assert(fword < size);
        assert(fpos < dstride);

        std::cout << "-I(" << __func__ << "): Generated Sa0Mask " << std::flush;
        for(size_t it=0; it<size; it++) {
            std::cout << " " << sa1Mask[it];
        }
        std::cout << std::endl;
#endif
    } 
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::applySeuFault()
{
    aux = dout.read();
    for(size_t it=0; it<size; it++) {
        aux[it] ^= seuMask[it];
    }
    dout.write(aux);

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-I(" << __func__ << "): Injecting SeuMask " << std::flush;
    for(size_t it=0; it<size; it++) {
        std::cout << " " << seuMask[it] << std::flush;
    }
    std::cout << std::endl;
#endif

    clearSeuMask();
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::applySa1Fault()
{
    aux = dout.read();
    for(size_t it=0; it<size; it++) {
        aux[it] |= sa1Mask[it];
    }
    dout.write(aux);
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::applySa0Fault()
{
    aux = dout.read();
    for(size_t it=0; it<size; it++) {
        aux[it] &= sa0Mask[it];
    }
    dout.write(aux);
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::clearSeuMask()
{
    for(size_t it=0; it<size; it++) {
        seuMask[it] = static_cast<ember::fault::mask_t>(0);
    }
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::clearSa1Mask()
{
    for(size_t it=0; it<size; it++) {
        sa1Mask[it] = static_cast<ember::fault::mask_t>(0);
    }
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::clearSa0Mask()
{
    for(size_t it=0; it<size; it++) {
        sa0Mask[it] = static_cast<ember::fault::mask_t>(~0);
    }
}


template<typename T> 
const char* ember::saboteur::priv::flipflop<T, true>::id()
{
    return name.c_str();
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::reset()
{
    aux.memset(0);
    dout.write(aux);
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::update()
{
    dout.write(din.read());
}

template<typename T> 
std::vector<ember::ISaboteur*> ember::saboteur::priv::flipflop<T, true>::getSaboteurs()
{
    std::vector<ember::ISaboteur*> aux;
    aux.push_back(this);
    return aux;
}

template<typename T> 
const size_t ember::saboteur::priv::flipflop<T, true>::locations(const ember::fault::model& fModel) const
{
    switch(fModel)
    {
        case ember::fault::model::seu:
        {
            return totalBw;
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
void ember::saboteur::priv::flipflop<T, true>::genFaultMask(const ember::fault::model& fModel)
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
void ember::saboteur::priv::flipflop<T, true>::clearFaultMask(const ember::fault::model& fModel)
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
void ember::saboteur::priv::flipflop<T, true>::clearAllMasks()
{
    clearSeuMask();
    clearSa0Mask();
    clearSa1Mask();
}

template<typename T> 
void ember::saboteur::priv::flipflop<T, true>::applyFault(const ember::fault::model& fModel)
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
void ember::saboteur::priv::flipflop<T, true>::applyAllFaults()
{
    applySeuFault();
    applySa0Fault();
    applySa1Fault();
}


template<typename T> 
ember::saboteur::priv::flipflop<T, true>::flipflop(const size_t& bitwidth)
    :   size(1),
        singleBw(bitwidth),
        totalBw(singleBw),
        name("reg"),
        din("din"),
        dout("dout"),
        aux(1, bitwidth)
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
ember::saboteur::priv::flipflop<T, true>::flipflop(const char* instanceId, const size_t& bitwidth)
    :   name(instanceId),
        size(1),
        singleBw(bitwidth),
        totalBw(singleBw),
        din("din", 1, bitwidth),
        dout("dout", 1, bitwidth),
        aux(1, bitwidth)
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
ember::saboteur::priv::flipflop<T, true>::flipflop(const char* instanceId, const size_t& len, const size_t& bitwidth)
    :   name(instanceId),
        size(len),
        singleBw(bitwidth),
        totalBw(len*singleBw),
        din("din", len, bitwidth),
        dout("dout", len, bitwidth),
        aux(len, bitwidth)
{
    // dout.bind();

    seuMask = new ember::fault::mask_t[len];
    sa0Mask = new ember::fault::mask_t[len];
    sa1Mask = new ember::fault::mask_t[len];

    clearSeuMask();
    clearSa0Mask();
    clearSa1Mask();

    resetCellList();
}


template<typename T> 
ember::saboteur::priv::flipflop<T, true>::~flipflop()
{
    delete[] seuMask;
    delete[] sa0Mask;
    delete[] sa1Mask;
}


#endif
