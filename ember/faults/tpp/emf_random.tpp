#include <cassert>
#include <cstddef>
#include <vector>
#include <type_traits>
#include "em_math.hpp"
#include "em_random.hpp"

namespace ember {
namespace fault {

template<FaultModel FM>
ISaboteur<FM>* uniform(const std::vector<ISaboteurBase*>& vec) {
    // keep only the saboteurs that support FM, and sum their weights
    std::vector<ISaboteur<FM>*> cands;
    size_t weightSum = 0;
    for (ISaboteurBase* p : vec) {
        if (auto* s = dynamic_cast<ISaboteur<FM>*>(p)) {
            cands.push_back(s);
            weightSum += s->locations(std::type_identity<FM>{});
        }
    }
    if (cands.empty()) return nullptr;

    size_t sel = ember::math::random::uniform((size_t)0, weightSum);

    for (auto* s : cands) {
        size_t w = s->locations(std::type_identity<FM>{});
        if (sel < w) {
            return s;
        }
        sel -= w;
    }

    assert(!"-E(Out of Scope): during random uniform component selection");
    return nullptr;
}

} // namespace fault
} // namespace ember