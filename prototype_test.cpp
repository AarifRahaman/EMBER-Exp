#include <cassert>
#include <concepts>
#include <iostream>
#include <type_traits>
#include <string_view>
#include <vector>
#include <variant>

enum class FaultTiming {            // This is an enum which has predefined three types of window
    Transient,
    Permanent,
};

 //Fault model concept


template <typename T>                                   // This will take a type      
concept beFaultModel = requires {                       // its a concept where type will checked at compile time and check whether all 
                                                        //the required contracts are satiesfied.
    {T::name} -> std::convertible_to<std::string_view>; //The type must have name which can be converted to any string_view type
    {T::timing} -> std::convertible_to<FaultTiming>;    // must have timing and that timing type is FaultTiming
    
    
};

//Example fault models


struct SEU
{
    static constexpr std::string_view name = "SEU";  // view works like const char* and also can do comparison, constexpr makes the name known at compile time as our SEU is a fixed name.

    static constexpr FaultTiming timing = FaultTiming::Transient; // Here timing is a member var and each of SEU object share the one timing, we can also do SEU::timing as we used static.

    
};


struct SA0
{
    static constexpr std::string_view name = "SA0";

    static constexpr FaultTiming timing = FaultTiming::Permanent;

    
};


struct SA1
{
    static constexpr std::string_view name = "SA1";

    static constexpr FaultTiming timing = FaultTiming::Permanent;

    
};

// This deliberately does NOT satisfy beFaultModel

struct FaultX
{
    static constexpr std::string_view name = "FaultX";

    using parameters = void;

};

// variadic ISaboteur


template <beFaultModel... FaultModels>
requires (sizeof...(FaultModels) > 0)
class ISaboteur {
protected:
    ISaboteur() = default;
public:

    using FaultVariant = std::variant<FaultModels ...>;     // using std::variant to hold all the FaultModels as a single type so that we can make FaultVariant& fModel
    
    virtual size_t locations(const FaultVariant& fModel) const = 0;
    virtual void genFaultMask(const FaultVariant& fModel) = 0;

    virtual void clearFaultMask(const FaultVariant& fModel) = 0;
    virtual void clearAllMasks() = 0;

    virtual void applyFault(const FaultVariant& fModel) = 0;
    virtual void applyAllFaults() = 0;

    virtual std::size_t faultModelCount() const = 0;

    // Other virtual functions.......

};


// Saboteur Base class 

template <beFaultModel... FaultModels>
class SaboteurBase : public ISaboteur<FaultModels...>{                

    public:

    // Number of fault models supported by this saboteur
    // Implementing virtual method

    std::size_t faultModelCount() const override
    {
        return sizeof...(FaultModels);
    }

    
    // Check whether a particular fault model is supported (assert(RegisterSaboteur::supports<SEU>());). But We can also make this for checking multiple at a time like, assert(RegisterSaboteur::supports<SEU,SA1,SA0>()); But i feels its redundant as we already have count().
    
    // It is a function template that can generate many different functions where virtual needs fixed signature.

    template <typename FaultModel>
    static constexpr bool supports()
    {
        return (std::same_as<FaultModel, FaultModels> || ...);
    }
};

// Implementation of DPSRAM aka Saboteur
// This is a concrete dpsram with 3 supported FM, but we can make dpsram templated if we wants something like, dpsram<SEU> a; dpsram<SEU, SA1> b; dpsram<SEU, SA1, SA0> c; user can do both

class dpsram : public SaboteurBase<SEU, SA0, SA1> {

    public:
    using FaultVariant = SaboteurBase<SEU, SA0, SA1>::FaultVariant;     // for concrete saboteur type, aliases is not actually needed as we inherited Base but here it just make it more understandable to the reader

    
    size_t totalBw = 64;                // for locations() purpose i declare these two var. Remember everything here is part of a prototype. Right now these are public on purpose.
    size_t freeSaLocations = 32;

    size_t locations(const FaultVariant& fModel) const override {

        return std::visit(                          // Here comes some complication of using variant. In existing dpsram you used enum model but here i used variant and to retrieve the active model from variant we need to use std::visit.
                                                    // i feels it works something like roundabout though but for keeping functions virtual , its a good way.
        
        [this](const auto& fault) -> size_t{        //This is a lambda function used to get the exact FaultModel instead of something like (const auto& SEU) to return available locations for every particular Fault.

        using T = std::decay_t<decltype(fault)>;    // Here T refers to the exact Fault like "SEU" or "SA1"

        if constexpr (std::same_as<T, SEU>)
        {
            return this -> totalBw;
        }
        else if constexpr (std::same_as<T, SA0>)
        {
            return this -> freeSaLocations;
        }
        else if constexpr (std::same_as<T, SA1>)
        {
            return this -> freeSaLocations;
        }
    }, fModel);
        
    }

    void genFaultMask(const FaultVariant&) override {}          // Dummy Implementation
    void clearFaultMask(const FaultVariant&) override {}
    void clearAllMasks() override {}

    void applyFault(const FaultVariant&) override {}
    void applyAllFaults() override {}

    
};

    

// Example saboteurs


class RegisterSaboteur : public SaboteurBase<SEU, SA0>
{   
    //Here must need to implement all the remaining virtual functions

    size_t locations(const FaultVariant& fModel) const override       // Dummy Implementation
    {
        return 0;
    }      
    void genFaultMask(const FaultVariant&) override {}          
    void clearFaultMask(const FaultVariant&) override {}
    void clearAllMasks() override {}

    void applyFault(const FaultVariant&) override {}
    void applyAllFaults() override {}
};

class MemorySaboteur : public SaboteurBase<SEU, SA0, SA1>
{
    size_t locations(const FaultVariant& fModel) const override       // Dummy Implementation
    {
        return 0;
    }      
    void genFaultMask(const FaultVariant&) override {}          
    void clearFaultMask(const FaultVariant&) override {}
    void clearAllMasks() override {}

    void applyFault(const FaultVariant&) override {}
    void applyAllFaults() override {}
};


// This is Prohibited

//class EmptySaboteur : public SaboteurBase<>
//{
//};


// Tests


int main()
{

    // Compile time tests

    static_assert(beFaultModel<SEU>);
    static_assert(beFaultModel<SA1>);
    static_assert(beFaultModel<SA0>);
    static_assert(!beFaultModel<FaultX>);


    
    
    // Runtime tests
    
    RegisterSaboteur reg1;
    RegisterSaboteur reg2;
    MemorySaboteur mem1;
    
    
    assert(reg1.faultModelCount() == 2);
    assert(RegisterSaboteur::supports<SEU>());      //supports() can be checked at compile time as well
    assert(!RegisterSaboteur::supports<SA1>());
    
    
    
    

    assert(mem1.faultModelCount() == 3);
    assert(MemorySaboteur::supports<SEU>());
    assert(MemorySaboteur::supports<SA1>());
    assert(MemorySaboteur::supports<SA0>());
    assert(!MemorySaboteur::supports<FaultX>());


    std::cout << "All Earlier prototype tests passed!\n";

    //End of Previous test

    
    // Test: FaultVariant type
    
    
    dpsram ram;

    using RamVariant = dpsram::FaultVariant;

    static_assert(std::same_as<RamVariant, std::variant<SEU, SA0, SA1>>);

    std::cout << "FaultVariant tests passed!\n";


    // Test: FaultVariant can hold each supported fault model

    RamVariant fault1 = SEU{};
    RamVariant fault2 = SA0{};
    RamVariant fault3 = SA1{};

    assert(std::holds_alternative<SEU>(fault1));        // This ensure if currently holding any SEU and correctly remember.
    assert(std::holds_alternative<SA0>(fault2));
    assert(std::holds_alternative<SA1>(fault3));

    std::cout << "FaultVariant storage tests passed!\n";


    // Test: locations() for SEU

    assert(ram.locations(SEU{}) == 64);

    std::cout << "SEU locations test passed!\n";


    // Test: locations() for SA0

    assert(ram.locations(SA0{}) == 32);

    std::cout << "SA0 locations test passed!\n";


    // Test: locations() for SA1

    assert(ram.locations(SA1{}) == 32);

    std::cout << "SA1 locations test passed!\n";


    // Test: locations() reacts to runtime state

    ram.totalBw = 128;
    assert(ram.locations(SEU{}) == 128);

    ram.freeSaLocations = 10;
    assert(ram.locations(SA0{}) == 10);
    assert(ram.locations(SA1{}) == 10);

    std::cout << "Runtime location-state tests passed!\n";


    // Test: faultModelCount() for dpsram

    assert(ram.faultModelCount() == 3);

    std::cout << "dpsram faultModelCount test passed!\n";


    // Test: compile-time supports() for dpsram

    static_assert(dpsram::supports<SEU>());
    static_assert(dpsram::supports<SA0>());
    static_assert(dpsram::supports<SA1>());
    static_assert(!dpsram::supports<FaultX>());

    std::cout << "dpsram supports tests passed!\n";


    // Test: access through ISaboteur base pointer
    // This specially checking virtual function behaviour.

    ISaboteur<SEU, SA0, SA1>* sab = &ram;

    RamVariant seuFault = SEU{};
    RamVariant sa0Fault = SA0{};
    RamVariant sa1Fault = SA1{};

    ram.totalBw = 64;
    ram.freeSaLocations = 32;

    assert(sab->locations(seuFault) == 64);
    assert(sab->locations(sa0Fault) == 32);
    assert(sab->locations(sa1Fault) == 32);

    std::cout << "Virtual dispatch tests passed!\n";


    // Test: variant changes active fault at runtime

    RamVariant currentFault = SEU{};

    assert(std::holds_alternative<SEU>(currentFault));
    assert(ram.locations(currentFault) == 64);

    currentFault = SA0{};

    assert(std::holds_alternative<SA0>(currentFault));
    assert(ram.locations(currentFault) == 32);

    currentFault = SA1{};

    assert(std::holds_alternative<SA1>(currentFault));
    assert(ram.locations(currentFault) == 32);

    std::cout << "Variant switching tests passed!\n";


        


    return 0;
}