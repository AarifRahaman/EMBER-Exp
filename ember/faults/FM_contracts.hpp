#pragma once

#include"emf_timing.hpp"

#include <concepts>
#include <cstddef>
#include <string_view>


template <typename T>
concept beFaultModel = requires {
    {T::name} -> std::convertible_to<std::string_view>;
    {T::timing} -> std::convertible_to<ember::fault::FaultTiming>;
    
};
