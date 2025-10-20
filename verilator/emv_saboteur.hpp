#ifndef __EMBER_VERILATOR_SABOTEUR_HPP__
#define __EMBER_VERILATOR_SABOTEUR_HPP__

#include <string>
#include <bitset>

#include "em_defs.hpp"
#include "em_isaboteur.hpp"
#include "emv_dtypes.hpp"
#include "emv_convert_c.hpp"

namespace ember
{

namespace verilator
{

//
//* === Verilated Signal / Variable Wrapper === *//
//

// Saboteur Helper
template<typename T, size_t bitwidth, bool is_extended, bool inplace = true>
requires(is_verilated<T>::value)
class saboteurHelper: public ISaboteur
{
public:
    const char* id();
    const size_t locations(const ember::fault::model& fModel) const;

    void genFaultMask(const ember::fault::model& fModel);

    void clearFaultMask(const ember::fault::model& fModel);
    void clearAllMasks();

    void applyFault(const ember::fault::model& fModel);
    void applyAllFaults();

    saboteurHelper(const char* _name, T* signal);
    ~saboteurHelper();
};

// Saboteur Helper - True
template<typename T, size_t bitwidth, bool inplace>
class saboteurHelper<T, bitwidth, true, inplace>: public ISaboteur
{
private:
    const std::string name;

    fault::mask_t* seuMask;
    bool isSeuFaulted;

    T* _data;

    void genSeuMask();
    void clearSeuMask();
    void applySeuFault();

public:
    const char* id();
    const size_t locations(const ember::fault::model& fModel) const;

    void genFaultMask(const ember::fault::model& fModel);

    void clearFaultMask(const ember::fault::model& fModel);
    void clearAllMasks();

    void applyFault(const ember::fault::model& fModel);
    void applyAllFaults();

    saboteurHelper(const char* _name, T* signal);
    ~saboteurHelper();
};

// Saboteur Helper - False
template<typename T, size_t bitwidth, bool inplace>
class saboteurHelper<T, bitwidth, false, inplace>: public ISaboteur
{
private:
    const std::string name;

    fault::mask_t seuMask;
    bool isSeuFaulted;

    T* _data;

    void genSeuMask();
    void clearSeuMask();
    void applySeuFault();

public:
    const char* id();
    const size_t locations(const ember::fault::model& fModel) const;

    void genFaultMask(const ember::fault::model& fModel);

    void clearFaultMask(const ember::fault::model& fModel);
    void clearAllMasks();

    void applyFault(const ember::fault::model& fModel);
    void applyAllFaults();

    saboteurHelper(const char* _name, T* signal);
    ~saboteurHelper();
};


// Saboteur
template<typename T, size_t bitwidth, bool inplace = true>
using saboteur = saboteurHelper<T, bitwidth, is_extended<T, bitwidth>::value, inplace>;

} // namespace verilator

} // namespace ember

#include "tpp/emv_nonext_saboteur.tpp"
#include "tpp/emv_extended_saboteur.tpp"

#endif