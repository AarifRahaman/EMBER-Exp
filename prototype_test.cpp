#include <cassert>
#include <concepts>
#include <iostream>
#include <type_traits>
#include <string_view>
#include <vector>

enum class FaultTiming {            // This is an enum which has predefined three types of window
    Transient,
    Permanent,
    Intermittent
};

 //Fault model concept


template <typename T>                                 // This will take a type      
concept beFaultModel = requires {                    // its a concept where type will checked at compile time and check whether all 
                                                    //the required contracts are satiesfied.
    {T::name} -> std::convertible_to<std::string_view>; //The type must have name which can be converted to any string_view type
    {T::timing} -> std::convertible_to<FaultTiming>;    // must have timing and that timing type is FaultTiming
    typename T::parameters;
    
};

//Example fault models


struct SEU
{
    static constexpr std::string_view name = "SEU";  // view works like const char* and also can do comparison, constexpr makes the name known at compile time as our SEU is a fixed name.

    static constexpr FaultTiming timing = FaultTiming::Transient; // Here timing is a member var and each of SEU object share the one timing, we can also do SEU::timing as we used static.

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


template <beFaultModel FaultModel>                  //Common interface as we need one common pointer type to store pointer inside vector container
class IFaultSaboteur
{
public:
    virtual ~IFaultSaboteur() = default;
};

// Saboteur Base class 

template <beFaultModel... FaultModels>
class SaboteurBase : public ISaboteur<FaultModels...>, public IFaultSaboteur<FaultModels>... {                // Inherits IFaultSaboteur as well

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
    
    RegisterSaboteur reg1;
    RegisterSaboteur reg2;
    MemorySaboteur mem1;
    
    assert(reg1.faultModelCount() == 2);
    assert(RegisterSaboteur::supports<SEU>());      //supports() can be checked at compile time as well
    assert(!RegisterSaboteur::supports<SA1>());
    
    

    assert(mem1.faultModelCount() == 3);
    assert(MemorySaboteur::supports<SEU>());
    assert(MemorySaboteur::supports<SA1>());
    assert(!MemorySaboteur::supports<FaultX>());


    std::cout << "All prototype tests passed!\n";

    //End of Previous test

    
    std::vector<IFaultSaboteur<SEU>*> seuBucket;        // Storing SEU/SA1 supported instances pointer in a vector container named seuBucket/sa1Bucket
    std::vector<IFaultSaboteur<SA1>*> sa1Bucket;

    seuBucket.push_back(&reg1);             // manually push the instance reference to the bucket
    seuBucket.push_back(&reg2);
    seuBucket.push_back(&mem1);

    sa1Bucket.push_back(&mem1);

    assert(seuBucket.size() == 3);
    assert(sa1Bucket.size() == 1);

    for (auto* sab : seuBucket)                 //Checking seuBucket contains pointer and none of them are null.
    {
    assert(sab != nullptr);

    std::cout << "Pointer: " << sab << '\n';    // Address of pointer
    }

    std::cout << "&reg1 = " << &reg1 << '\n';      // we get the different address compare to 'sab' because full object has one starting address but bucket-
                                                //point specifically to SEU compatible base part.
    std::cout << "&reg2 = " << &reg2 << '\n';
    std::cout << "&mem1 = " << &mem1 << '\n';

    std::cout << "SEU bucket contains "         //cheking the size of bucket
            << seuBucket.size()
            << " saboteurs\n";
    
    std::cout << "SA1 bucket contains "
            << sa1Bucket.size()
            << " saboteurs\n";

    return 0;
}