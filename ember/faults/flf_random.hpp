#ifndef __EMBER_FAULT_RANDOM_HPP__
#define __EMBER_FAULT_RANDOM_HPP__

#include <vector>

#include "em_imutant.hpp"
#include "emf_baseline_dtypes.hpp"

namespace ember {

namespace fault
{

// Weighted Uniform Distribution over components size
IMutant* uniform(std::vector<IMutant*> vec, const model& fModel);

} // namespace fault

} // namespace ember

#endif