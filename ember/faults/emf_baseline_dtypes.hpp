#pragma once

#include "em_isaboteur.hpp"
#include "em_tick.hpp"
#include "em_types.hpp"

#include <sstream>
#include <string>

namespace ember {

namespace fault {

//
// == Baseline Fault Models == //
//
enum model {
    seu, // Single-Event Upset (SEU)
    sa1, // Stuck-At '1' (SA1)
    sa0  // Stuck-At '0' (SA0)
};

//
// == Default Mask Type == //
//
using mask_t = ember::uint64_t;

} // namespace fault

} // namespace ember
