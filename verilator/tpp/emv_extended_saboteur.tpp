#ifndef __EMBER_VERILATOR_EXTENDED_SABOTEUR_TPP__
#define __EMBER_VERILATOR_EXTENDED_SABOTEUR_TPP__

template<typename T, size_t bitwidth, bool inplace>
const char* ember::verilator::saboteurHelper<T, bitwidth, true, inplace>::id()
{
    return name.c_str();
}

template<typename T, size_t bitwidth, bool inplace>
const size_t ember::verilator::saboteurHelper<T, bitwidth, true, inplace>::locations(const ember::fault::model& fModel) const
{
    return bitwidth;
}

template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, true, inplace>::genSeuMask()
{
    size_t bit   = ember::math::random::uniform<>((unsigned) 0, (unsigned) bitwidth);
    size_t it    = bit / bitsizeof(T);
    size_t pos   = bit % bitsizeof(T);
    seuMask[it]  = 0x1 << pos;
    isSeuFaulted = true;

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-I(" << __func__ << "): Generated SeuMask:" << std::endl;
    for(size_t it=0; it<ember::verilator::extended<T, bitwidth>::len; it++) {
        std::bitset<bitsizeof(T)> bseuMask(seuMask[it]);
        std::cout << " " << bseuMask << std::flush;
    }
    std::cout << std::endl;
#endif
}
template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, true, inplace>::genFaultMask(const ember::fault::model& fModel)
{
    switch(fModel) {
        case ember::fault::model::seu:
        {
            genSeuMask();
            break;
        }
        default:
        {
            throw std::invalid_argument("fModel is not supported");
        }
    }
}


template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, true, inplace>::clearSeuMask()
{
    for(size_t it=0; it<ember::verilator::extended<T, bitwidth>::len; it++) { seuMask[it] = 0; }
    isSeuFaulted = false;
}

template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, true, inplace>::clearFaultMask(const ember::fault::model& fModel)
{
    switch(fModel) {
        case ember::fault::model::seu:
        {
            clearSeuMask();
            break;
        }
        default:
        {
            throw std::invalid_argument("fModel is not supported");
        }
    }
}

template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, true, inplace>::clearAllMasks()
{
    clearSeuMask();
}


template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, true, inplace>::applySeuFault()
{
#if defined(DEBUG) || defined(VERBOSE)
    if(isSeuFaulted) {
        std::cout << "-I(" << __func__ << "): pre-injection data: " << std::endl;
        for(size_t it=0; it<ember::verilator::extended<T, bitwidth>::len; it++) {
            std::cout << " " << _data[it] << std::flush;
        }
        std::cout << std::endl;
    }
#endif

    if(isSeuFaulted) {
        if constexpr(inplace) {
            for(size_t it=0; it<ember::verilator::extended<T, bitwidth>::len; it++) {
                _data[it] ^= seuMask[it];
            }
        }
        else {
            for(size_t it=0; it<ember::verilator::extended<T, bitwidth>::len; it++) {
                _data[it] = seuMask[it];
            }
        }
    }

#if defined(DEBUG) || defined(VERBOSE)
    // if(isSeuFaulted) {
    //     std::cout << "-I(" << __func__ << "): Injecting SeuMask: " << std::endl;
    //     for(size_t it=0; it<ember::verilator::extended<T, bitwidth>::len; it++) {
    //         std::bitset<bitsizeof(T)> bseuMask(seuMask[it]);
    //         std::cout << " " << bseuMask << std::flush;
    //     }
    // }

    if(isSeuFaulted) {
        std::cout << "-I(" << __func__ << "): post-injection data: " << std::endl;
        for(size_t it=0; it<ember::verilator::extended<T, bitwidth>::len; it++) {
            std::cout << " " << _data[it] << std::flush;
        }
        std::cout << std::endl;
    }
#endif

    // Clear Temporary Masks
    clearSeuMask();
}

template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, true, inplace>::applyFault(const ember::fault::model& fModel)
{
    switch(fModel) {
        case ember::fault::model::seu:
        {
            applySeuFault();
            break;
        }
        default:
        {
            throw std::invalid_argument("fModel is not supported");
        }
    }
}

template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, true, inplace>::applyAllFaults()
{
    applySeuFault();
}


template<typename T, size_t bitwidth, bool inplace>
ember::verilator::saboteurHelper<T, bitwidth, true, inplace>::saboteurHelper(const char* _name, T* signal)
{
    _data        = signal;
    seuMask      = new ember::fault::mask_t[ember::verilator::extended<T, bitwidth>::len];
    clearSeuMask();
}

template<typename T, size_t bitwidth, bool inplace>
ember::verilator::saboteurHelper<T, bitwidth, true, inplace>::~saboteurHelper()
{
    delete[] seuMask;
}


#endif