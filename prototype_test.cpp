#include <cassert>
#include <concepts>
#include <iostream>
#include <type_traits>


enum class FaultTiming {
    Transient,
    Permanent,
    Intermittent
};

 //Fault model concept


template <typename T>
concept beFaultModel = requires {
    {T::name} -> std::convertible_to<const char*>;
    {T::timing} -> std::convertible_to<FaultTiming>;
    {T::multiplicity} -> std::convertible_to<std::size_t>;
    typename T::parameters;
    
};

//Example fault models


struct SEU
{
    static constexpr const char* name = "SEU";

    static constexpr FaultTiming timing = FaultTiming::Transient;

    static constexpr std::size_t multiplicity = 2;

    using parameters = void;
};


struct SA0
{
    static constexpr const char* name = "SA0";

    static constexpr FaultTiming timing = FaultTiming::Permanent;

    static constexpr std::size_t multiplicity = 1;

    using parameters = void;
};


struct SA1
{
    static constexpr const char* name = "SA1";

    static constexpr FaultTiming timing = FaultTiming::Permanent;

    static constexpr std::size_t multiplicity = 1;

    using parameters = void;
};
// This deliberately does NOT satisfy beFaultModel

struct FaultX
{
    static constexpr const char* name = "FaultX";

    static constexpr FaultTiming timing = FaultTiming::Intermittent;

    static constexpr std::size_t multiplicity = 1;
};

// variadic ISaboteur


template <beFaultModel... FaultModels>
requires (sizeof...(FaultModels) > 0)
class ISaboteur
{

    public:
    // This is our current scenario
    //virtual const size_t locations(const ember::fault::model& fModel) const = 0;

    // I wanted to do something like that but its not something logical.
    template <beFaultModel FaultModels>
    const size_t locations(const FaultModels& fModel) const
    {
       
    }

    // Number of fault models supported by this saboteur
    static constexpr std::size_t faultModelCount()
    {
        return sizeof...(FaultModels);
    }

    // Check whether a particular fault model is supported
    
    template <typename FaultModel>
    static constexpr bool supports()
    {
        return (std::same_as<FaultModel, FaultModels> || ...);
    }
};

// Example saboteurs


class RegisterSaboteur : public ISaboteur<SEU, SA0>
{
};

class MemorySaboteur : public ISaboteur<SEU, SA0, SA1>
{
};

// This is Prohibited

//class EmptySaboteur : public ISaboteur<>
//{
//};

// Tests


int main()
{
    
    // Test 1: number of supported fault models


    static_assert(RegisterSaboteur::faultModelCount() == 2);
    static_assert(MemorySaboteur::faultModelCount() == 3);
    

    
    // Test 2: RegisterSaboteur support
    

    static_assert(RegisterSaboteur::supports<SEU>());
    static_assert(RegisterSaboteur::supports<SA0>());
    static_assert(!RegisterSaboteur::supports<SA1>());

    
    // Test 3: MemorySaboteur support
    

    static_assert(MemorySaboteur::supports<SEU>());
    static_assert(MemorySaboteur::supports<SA0>());
    static_assert(MemorySaboteur::supports<SA1>());


    
    // Runtime sanity tests
    

    assert(RegisterSaboteur::faultModelCount() == 2);
    assert(RegisterSaboteur::supports<SEU>());
    assert(!RegisterSaboteur::supports<SA1>());

    std::cout << "All prototype tests passed!\n";

    return 0;
}