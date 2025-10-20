#ifndef __EMBER_VERILATOR_NONEXT_SABOTEUR_TPP__
#define __EMBER_VERILATOR_NONEXT_SABOTEUR_TPP__

template<typename T, size_t bitwidth, bool inplace>
const char* ember::verilator::saboteurHelper<T, bitwidth, false, inplace>::id()
{
    return name.c_str();
}

template<typename T, size_t bitwidth, bool inplace>
const size_t ember::verilator::saboteurHelper<T, bitwidth, false, inplace>::locations(const ember::fault::model& fModel) const
{
    return bitwidth;
}

template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, false, inplace>::genSeuMask()
{
    size_t pos = ember::math::random::uniform<>((unsigned) 0, (unsigned) bitwidth);
    seuMask = 0x1 << pos;
    isSeuFaulted = true;

#if defined(DEBUG) || defined(VERBOSE)
    std::cout << "-I(" << __func__ << "): Generated SeuMask " << seuMask << std::endl;
#endif
}
template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, false, inplace>::genFaultMask(const ember::fault::model& fModel)
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
void ember::verilator::saboteurHelper<T, bitwidth, false, inplace>::clearSeuMask()
{
    seuMask = 0;
    isSeuFaulted = false;
}

template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, false, inplace>::clearFaultMask(const ember::fault::model& fModel)
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
void ember::verilator::saboteurHelper<T, bitwidth, false, inplace>::clearAllMasks()
{
    clearSeuMask();
}


template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, false, inplace>::applySeuFault()
{
    if(isSeuFaulted) {
        if constexpr(inplace) {
            *_data ^= seuMask;
        }
        else {
            *_data = seuMask;
        }
    }

#if defined(DEBUG) || defined(VERBOSE)
    if(isSeuFaulted) {
        std::bitset<bitsizeof(T)> bseuMask(seuMask);
        std::cout << "-I(" << __func__ << "): Injecting SeuMask " << bseuMask << std::endl;
    }
#endif

    // Clear Temporary Masks
    clearSeuMask();
}

template<typename T, size_t bitwidth, bool inplace>
void ember::verilator::saboteurHelper<T, bitwidth, false, inplace>::applyFault(const ember::fault::model& fModel)
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
void ember::verilator::saboteurHelper<T, bitwidth, false, inplace>::applyAllFaults()
{
    applySeuFault();
}


template<typename T, size_t bitwidth, bool inplace>
ember::verilator::saboteurHelper<T, bitwidth, false, inplace>::saboteurHelper(const char* _name, T* signal)
{
    _data = signal;
    clearSeuMask();
}

template<typename T, size_t bitwidth, bool inplace>
ember::verilator::saboteurHelper<T, bitwidth, false, inplace>::~saboteurHelper() { }


#endif