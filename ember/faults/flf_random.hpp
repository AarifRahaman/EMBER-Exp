#pragma once

#include "em_imutant.hpp"
#include "emf_baseline_dtypes.hpp"

#include <vector>

namespace ember {

namespace fault {

// Weighted Uniform Distribution over components size
IMutant* uniform(std::vector<IMutant*> vec, const model& fModel);

} // namespace fault

} // namespace ember
