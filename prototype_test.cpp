#include <cassert>
#include <concepts>
#include <iostream>
#include <type_traits>
#include <string_view>
#include <vector>
#include <variant>
#include <bitset>
#include <random>

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



// Example saboteurs


class RegisterSaboteur : public SaboteurBase<SEU, SA0>
{   
    //Here must need to implement all the remaining virtual functions
    size_t locations(const FaultVariant& fModel) const override
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
    public:
    size_t freesalocations;
    size_t totalbw;
    using mask_t = uint8_t;

    mask_t seuMask = 0;
    mask_t sa1Mask = 0;
    mask_t sa0Mask = ~mask_t{0};

    uint8_t data = 0;        // Memory location of simulated hardware

    size_t uniformPosition()
    {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<size_t> dis(0, totalbw - 1);

    return dis(gen);
    }

    void clearseuMask()
    {
        seuMask = 0;
    }

     void clearsa1Mask()
    {
        sa1Mask = 0;
    }

     void clearsa0Mask()
    {
        sa0Mask = ~mask_t{0};
    }

    
    void applyseuFault()
    {
        data ^= seuMask;
    }
    
    void applysa1Fault()
    {
        data |= sa1Mask;
    }
    
    void applysa0Fault()
    {
        data &= sa0Mask;
    }

    
    size_t locations(const FaultVariant& fModel) const override {

        assert(freesalocations <= totalbw);
        
        return std::visit(                          
                                                    
        
        [this](const auto& fault) -> size_t{        

        using T = std::decay_t<decltype(fault)>;    

        if constexpr (std::same_as<T, SEU>)
        {
            size_t result = this -> totalbw;
            
            #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << ": SEU locations = "
                        << result << '\n';
            #endif

            return result;
        }

        
        else if constexpr (std::same_as<T, SA0>)
        {
            size_t result =  this -> freesalocations;

            #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << ": SA0 locations = "
                        << result << '\n';
            #endif

            return result;

        }
        else if constexpr (std::same_as<T, SA1>)
        {
            size_t result =  this -> freesalocations;

            #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << ": SA1 locations = "
                        << result << '\n';
            #endif
            
            return result;
        }
    },fModel); 

    }   
    
    void genFaultMask(const FaultVariant& fmodel) override 
    {
        
        std::visit(                          
                                                    
        
        [this](const auto& fault) -> void{        

        using T = std::decay_t<decltype(fault)>;    

        size_t pos = uniformPosition();

        if constexpr (std::same_as <T, SEU>)
        {
           seuMask |= (mask_t{1} << pos);

           #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << "Generated SEU mask at Position: "
                        <<pos
                        <<" & Mask = "
                        <<std::bitset<8>(seuMask)
                        <<'\n';
            #endif
        }

        else if constexpr (std::same_as <T, SA1>)
        {
            sa1Mask |= (mask_t{1} << pos);

            #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << "Generated SA1 mask at Position: "
                        <<pos
                        <<" & Mask = "
                        <<std::bitset<8>(sa1Mask)
                        <<'\n';
            #endif
        }

         else if constexpr (std::same_as <T, SA0>)
        {
            sa0Mask &= ~(mask_t{1} << pos);

            #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << "Generated SA0 mask at Position: "
                        <<pos
                        <<" & Mask = "
                        <<std::bitset<8>(sa0Mask)
                        <<'\n';
            #endif
        }

       
        } ,
        fmodel); 
    }       
    void clearFaultMask(const FaultVariant& fmodel) override 
    {
        std::visit(                          
                                                    
        
        [this](const auto& fault) -> void{        

        using T = std::decay_t<decltype(fault)>;    

        if constexpr (std::same_as <T, SEU>)
        {
           clearseuMask();

           #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << ": SEU Mask Cleared = "
                        <<std::bitset<8>(seuMask)
                        <<'\n';
            #endif
           

        }

        else if constexpr (std::same_as <T, SA1>)
        {
            clearsa1Mask();

            #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << ": SA1 Mask Cleared = "
                        <<std::bitset<8>(sa1Mask)
                        <<'\n';
            #endif
        }

         else if constexpr (std::same_as <T, SA0>)
        {
            clearsa0Mask();

            #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << ": SA0 Mask Cleared = "
                        <<std::bitset<8>(sa0Mask)
                        <<'\n';
            #endif
        }

       
        } ,
        fmodel); 
    }
    void clearAllMasks() override 
    {
        clearsa0Mask();
        clearsa1Mask();
        clearseuMask();

        assert(seuMask == 0);
        assert(sa1Mask == 0);
        assert(sa0Mask == 0xFF);

        #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << ": All Mask Cleared "
                        <<'\n';
        #endif
    }

    void applyFault(const FaultVariant& fmodel) override 
    {
        std::visit(                          
                                                    
        
        [this](const auto& fault) -> void{        

        using T = std::decay_t<decltype(fault)>;    

        mask_t before = data;

        if constexpr (std::same_as <T, SEU>)
        {
            
            applyseuFault();

            assert(data == (before ^ seuMask));

            #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << ": SEU applied"
                        << "\nData before: " << std::bitset<8>(before)
                        << "\nSEU mask:    " << std::bitset<8>(seuMask)
                        << "\nData after:  " << std::bitset<8>(data)
                        << '\n';
            #endif
        }

        else if constexpr (std::same_as <T, SA1>)
        {
            applysa1Fault();

            assert(data == (before | sa1Mask));

            #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << ": SA1 applied"
                        << "\nData before: " << std::bitset<8>(before)
                        << "\nSA1 mask:    " << std::bitset<8>(sa1Mask)
                        << "\nData after:  " << std::bitset<8>(data)
                        << '\n';
            #endif
        }

         else if constexpr (std::same_as <T, SA0>)
        {
            applysa0Fault();

            assert(data == (before & sa0Mask));

            #ifdef DEBUG
            std::cout   << "[DEBUG] " << __func__
                        << ": SA0 applied"
                        << "\nData before: " << std::bitset<8>(before)
                        << "\nSA0 mask:    " << std::bitset<8>(sa0Mask)
                        << "\nData after:  " << std::bitset<8>(data)
                        << '\n';
            #endif
        }

       
        } ,
        fmodel); 
    }
    void applyAllFaults() override 
    {
        mask_t before = data;

        applyseuFault();
        applysa1Fault();
        applysa0Fault();

        #ifdef DEBUG
        std::cout   << "[DEBUG] " << __func__
                    << ": All faults applied"
                    << "\nData before: " << std::bitset<8>(before)
                    << "\nData after:  " << std::bitset<8>(data)
                    << '\n';
        #endif
    }

    //CTOR

    MemorySaboteur(size_t freesa, size_t total)
    : freesalocations(freesa),
      totalbw(total)
{
    assert(totalbw > 0);
    assert(totalbw <= 8);
    assert(freesalocations <= totalbw);
}


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
    MemorySaboteur mem1 (8,8);
    
    
    
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


    // Example saboteur test.

    MemorySaboteur ex(8,8);
    ex.data = 0b10100101;

    MemorySaboteur::FaultVariant seu = SEU{};
    MemorySaboteur::FaultVariant sa0 = SA0{};
    MemorySaboteur::FaultVariant sa1 = SA1{};

    ex.locations(seu);
    ex.locations(sa0);
    ex.locations(sa1);

    
    ex.genFaultMask(seu);
    ex.genFaultMask(sa1);
    ex.genFaultMask(sa0);
    
    ex.applyAllFaults();

    ex.clearAllMasks();


        


    return 0;
}