#include <cassert>
#include <concepts>
#include <iostream>
#include <type_traits>
#include <string_view>


enum class FaultTiming {
    Transient,
    Permanent,
    Intermittent
};

 //Fault model concept


template <typename T>
concept beFaultModel = requires {
    {T::name} -> std::convertible_to<std::string_view>;
    {T::timing} -> std::convertible_to<FaultTiming>;
    typename T::parameters;
    
};

//Example fault models


struct SEU
{
    static constexpr std::string_view name = "SEU";  // view works like const char* and also can do comparison, constexpr makes the name known at compile time as our SEU is a fixed name.

    static constexpr FaultTiming timing = FaultTiming::Transient; // Here timing is a member var and each of SEU object share the one timing, we can also do SEU::timing.

    using parameters = void;
};


struct SA0
{
    static constexpr std::string_view name = "SA0";

    static constexpr FaultTiming timing = FaultTiming::Permanent;

    using parameters = void;
};


struct SA1
{
    static constexpr std::string_view name = "SA1";

    static constexpr FaultTiming timing = FaultTiming::Permanent;

    using parameters = void;
};

// This deliberately does NOT satisfy beFaultModel

struct FaultX
{
    static constexpr std::string_view name = "FaultX";

    static constexpr FaultTiming timing = FaultTiming::Intermittent;

};

// variadic ISaboteur


template <beFaultModel... FaultModels>
requires (sizeof...(FaultModels) > 0)
class ISaboteur {
protected:
    ISaboteur() = default;
public:
    virtual ~ISaboteur() = default;

    virtual std::size_t faultModelCount() const = 0;

    // Other virtual functions.......

};


// Saboteur Base class 

template <beFaultModel... FaultModels>
class SaboteurBase : public ISaboteur<FaultModels...> {

    public:

    // Number of fault models supported by this saboteur
    // Implementing virtual method

    std::size_t faultModelCount() const override
    {
        return sizeof...(FaultModels);
    }

    // Check whether a particular fault model is supported
    // It is a function template that can generate many different functions where virtual needs fixed signature.

    template <typename FaultModel>
    static constexpr bool supports()
    {
        return (std::same_as<FaultModel, FaultModels> || ...);
    }
};



// Example saboteurs


class RegisterSaboteur : public SaboteurBase<SEU, SA0>
{   
    //Here must need to implement all the remaining virtual functions
};

class MemorySaboteur : public SaboteurBase<SEU, SA0, SA1>
{
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
    
    RegisterSaboteur reg;
    assert(reg.faultModelCount() == 2);
    
    assert(RegisterSaboteur::supports<SEU>());
    assert(!RegisterSaboteur::supports<SA1>());
    
    MemorySaboteur mem;

    assert(mem.faultModelCount() == 3);
    assert(MemorySaboteur::supports<SEU>());
    assert(MemorySaboteur::supports<SA1>());
    assert(!MemorySaboteur::supports<FaultX>());


    std::cout << "All prototype tests passed!\n";

    return 0;
}