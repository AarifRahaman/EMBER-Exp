#pragma once

#include "em_isaboteurbase.hpp"
#include "emf_baseline_dtypes.hpp"
#include "FM_contracts.hpp"

#include <vector>

namespace ember {

namespace fault {

// Weighted Uniform Distribution over components size
template<FaultModel FM>
ISaboteur<FM>* uniform(const std::vector<ISaboteurBase*>& vec);

} // namespace fault

} // namespace ember

#include "tpp/emf_random.tpp"
