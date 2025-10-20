#ifndef __EMBER_FAULT_BASELINE_DTYPES_HPP__
#define __EMBER_FAULT_BASELINE_DTYPES_HPP__

#include <string>
#include <sstream>

#include "em_types.hpp"
#include "em_time.hpp"
#include "em_isaboteur.hpp"

namespace ember {

namespace fault {

//
// == Baseline Fault Models == //
//
enum model {
    seu,    // Single-Event Upset (SEU)
    sa1,    // Stuck-At '1' (SA1)
    sa0     // Stuck-At '0' (SA0)
};


//
// == Default Mask Type == //
//
using mask_t = ember::uint64_t;


}

}

#endif