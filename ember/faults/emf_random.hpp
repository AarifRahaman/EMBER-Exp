#ifndef __EMBER_FAULT_RANDOM_HPP__
#define __EMBER_FAULT_RANDOM_HPP__

#include <vector>

#include "em_isaboteur.hpp"
#include "emf_baseline_dtypes.hpp"

namespace ember {

namespace fault
{

// Weighted Uniform Distribution over components size
ISaboteur* uniform(std::vector<ISaboteur*> vec, const model& fModel);

} // namespace fault

} // namespace ember

#endif