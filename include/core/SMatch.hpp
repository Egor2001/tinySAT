#ifndef TINYSAT_SMATCH_HPP_
#define TINYSAT_SMATCH_HPP_

#include <cstdint>

#include <vector>
#include <iostream>

namespace tinysat {

struct SMatch;

inline bool operator == (const SMatch& lhs, const SMatch& rhs);
inline bool operator != (const SMatch& lhs, const SMatch& rhs);

template<typename TStream>
TStream& operator << (TStream&, const SMatch&);

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

inline bool operator == (const SMatch& lhs, const SMatch& rhs)
{
    return lhs.value_vec == rhs.value_vec;
}

inline bool operator != (const SMatch& lhs, const SMatch& rhs)
{
    return !(lhs == rhs);
}

template<typename TStream>
TStream& operator << (TStream& stream, const SMatch& match)
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

#endif // TINYSAT_SMATCH_HPP_

