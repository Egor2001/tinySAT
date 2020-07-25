#ifndef TINYSAT_SFORMULA_HPP_
#define TINYSAT_SFORMULA_HPP_

#include <cstdint>

#include <vector>
#include <iostream>

#include "SMatch.hpp"

namespace tinysat {

struct SFormula
{
    [[nodiscard]] bool is_match(const SMatch&) const;

    size_t params_cnt;
    std::vector<std::vector<int32_t>> clause_vec;
};

std::ostream& operator << (std::ostream&, const SFormula&);

} // namespace tinysat

#endif // TINYSAT_SFORMULA_HPP_
