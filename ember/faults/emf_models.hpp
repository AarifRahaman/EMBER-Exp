#pragma once

#include "em_isaboteur.hpp"
#include "emf_dtypes.hpp"
#include "em_tick.hpp"
#include "emf_timing.hpp"
#include <string_view>

namespace ember {

namespace fault {

//
// == SEU Class == //
//
class seu_t : public fault_t<model> {

public:
    static constexpr std::string_view name = "SEU";
    static constexpr FaultTiming timing = FaultTiming::transient;

    seu_t& operator=(const fault_t<model>& other);

    seu_t(const sim::Tick fault_time, ISaboteurBase* location);
    seu_t(const fault_t<model>& other);
};

//
// == Stuck-At-0 Classes == //
//
class sa0_t : public fault_t<model> {

public:
    static constexpr std::string_view name = "SA0";
    static constexpr FaultTiming timing = FaultTiming::permanent;

    sa0_t& operator=(const fault_t<model>& other);

    sa0_t(const sim::Tick fault_time, ISaboteurBase* location);
    sa0_t(const fault_t<model>& other);
};

//
// == Stuck-At-1 Classes == //
//

class sa1_t : public fault_t<model> {

public:
    static constexpr std::string_view name = "SA1";
    static constexpr FaultTiming timing = FaultTiming::permanent;

    sa1_t& operator=(const fault_t<model>& other);

    sa1_t(const sim::Tick fault_time, ISaboteurBase* location);
    sa1_t(const fault_t<model>& other);
};

} // namespace fault

} // namespace ember