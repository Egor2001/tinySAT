#ifndef TINYSAT_SMATCH_HPP_
#define TINYSAT_SMATCH_HPP_

#include <cstdint>

#include <vector>
#include <iostream>

namespace tinysat {

struct SMatch
{
    enum class EValue : uint8_t
    {
        FALSE = 0,
        TRUE = 1,
        NONE = 2
    };

    std::vector<EValue> value_vec;
};

bool operator == (const SMatch& lhs, const SMatch& rhs);
bool operator != (const SMatch& lhs, const SMatch& rhs);

std::ostream& operator << (std::ostream&, const SMatch&);

} // namespace tinysat

#endif // TINYSAT_SMATCH_HPP_
