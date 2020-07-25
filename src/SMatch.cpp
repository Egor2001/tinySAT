#include "SMatch.hpp"

namespace tinysat {

bool operator == (const SMatch& lhs, const SMatch& rhs)
{
    return lhs.value_vec == rhs.value_vec;
}

bool operator != (const SMatch& lhs, const SMatch& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator << (std::ostream& stream, const SMatch& match)
{
    stream << "[ ";
    for (const auto& val : match.value_vec)
    {
        switch (val)
        {
            case SMatch::EValue::TRUE: stream << "T "; break;
            case SMatch::EValue::FALSE: stream << "F "; break;

            case SMatch::EValue::NONE: [[fallthrough]];
            default: stream << "X "; break;
        }
    }

    stream << "]";

    return stream;
}

} // namespace tinysat
