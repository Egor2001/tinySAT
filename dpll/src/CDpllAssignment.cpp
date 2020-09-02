#include "CDpllAssignment.hpp"

/**
 * @file CDpllAssignment.cpp
 * @author geome_try
 * @date 2020
 * @see CDpllAssignment.hpp
 */

/// @brief
namespace tinysat {

/**
 * @param [in] formula SAT formula to assign priorities from
 */
CDpllAssignment::CDpllAssignment(const SFormula& formula):
    match_(),
    sort_heap_(formula),
    log_stk_()
{
    match_.value_vec = std::vector(formula.params_cnt, SMatch::EValue::NONE);
}

/**
 * @return state for backtracking
 * @see backtrack()
 */
CDpllAssignment::SState CDpllAssignment::get_state() const
{
    return SState {
        .log_idx = log_stk_.size()
    };
}

/**
 * @param [in] state for backtracking
 * @see get_state()
 */
void CDpllAssignment::backtrack(const SState& state)
{
    while (state.log_idx < log_stk_.size())
    {
        revert(log_stk_.top());
        log_stk_.pop();
    }
}

/**
 * @return literal with the highest priority
 * @see proceed()
 */
int CDpllAssignment::request() const
{
    return (sort_heap_.empty() ? 0 : sort_heap_.get());
}

/**
 * @param [in] lit literal to assign true to
 * @see request()
 */
void CDpllAssignment::proceed(const int lit)
{
    log_stk_.push(lit);
    assign(lit);
}

/**
 * @return true if no errors detected
 * @see dump()
 */
bool CDpllAssignment::ok() const noexcept
{
    return sort_heap_.ok();
}

/**
 * @param [in,out] stream iostream-like stream object 
 * @return ok()
 * @see ok()
 */
template<typename TStream>
bool CDpllAssignment::dump(TStream& stream) const noexcept
{
    stream << match_ << "\n";
    sort_heap_.dump(stream);

    return ok();
}

/**
 * @param [in] lit literal to assign true to
 * @see revert()
 */
void CDpllAssignment::assign(const int lit)
{
    if (lit == 0)
        throw CException("error: assigning zero literal");

    if (match_.value_vec[std::abs(lit) - 1u] != SMatch::EValue::NONE)
        throw CException("error: reassigning literal");

    match_.value_vec[std::abs(lit) - 1u] = 
        (lit < 0 ? SMatch::EValue::FALSE : SMatch::EValue::TRUE);

    sort_heap_.extract(lit);
    sort_heap_.extract(-lit);
}

/**
 * @param [in] lit literal to revert assignment
 * @see assign()
 */
void CDpllAssignment::revert(const int lit)
{
    if (lit == 0)
        throw CException("error: reverting zero literal");

    if (match_.value_vec[std::abs(lit) - 1u] == SMatch::EValue::NONE)
        throw CException("error: reverting not assigned literal");

    sort_heap_.restore(-lit);
    sort_heap_.restore(lit);

    sort_heap_.dec_prior(lit);

    match_.value_vec[std::abs(lit) - 1u] = SMatch::EValue::NONE;
}

} // namespace tinysat
