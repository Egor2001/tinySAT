#ifndef TINYSAT_DPLL_CDPLLASSIGNMENT_HPP_
#define TINYSAT_DPLL_CDPLLASSIGNMENT_HPP_

#include "misc/CChangeLog.hpp"

#include "CException.hpp"
#include "SFormula.hpp"
#include "SMatch.hpp"
#include "CDpllSortHeap.hpp"

#include "../src/SFormula.cpp"
#include "../src/SMatch.cpp"

namespace tinysat {

class CDpllAssignment
{
public:
    struct SState
    {
        size_t log_idx;
    };

    CDpllAssignment() = default;
    explicit CDpllAssignment(const SFormula&);

    CDpllAssignment             (const CDpllAssignment&) = default;
    CDpllAssignment& operator = (const CDpllAssignment&) = default;
    CDpllAssignment             (CDpllAssignment&&) = default;
    CDpllAssignment& operator = (CDpllAssignment&&) = default;

    [[nodiscard]] const SMatch& match() const noexcept
    {
        return match_;
    }

    [[nodiscard]] SState get_state() const;
    void backtrack(const SState&);

    int request() const;
    void proceed(const int);

    [[nodiscard]] bool full() const
    {
        return (request() == 0);
    }

    [[nodiscard]] bool ok() const noexcept;

    template<typename TStream>
    bool dump(TStream& stream) const noexcept;

protected:
    void assign(const int);
    void revert(const int);

private:
    SMatch match_;
    CDpllSortHeap sort_heap_;
    std::stack<int> log_stk_;
};

CDpllAssignment::CDpllAssignment(const SFormula& formula):
    match_(),
    sort_heap_(formula),
    log_stk_()
{
    match_.value_vec = std::vector(formula.params_cnt, SMatch::EValue::NONE);
}

CDpllAssignment::SState CDpllAssignment::get_state() const
{
    return SState {
        .log_idx = log_stk_.size()
    };
}

void CDpllAssignment::backtrack(const SState& state)
{
    while (state.log_idx < log_stk_.size())
    {
        revert(log_stk_.top());
        log_stk_.pop();
    }
}

int CDpllAssignment::request() const
{
    return (sort_heap_.empty() ? 0 : sort_heap_.get());
}

void CDpllAssignment::proceed(const int lit)
{
    log_stk_.push(lit);
    assign(lit);
}

bool CDpllAssignment::ok() const noexcept
{
    return sort_heap_.ok();
}

template<typename TStream>
bool CDpllAssignment::dump(TStream& stream) const noexcept
{
    stream << match_ << "\n";
    sort_heap_.dump(stream);

    return ok();
}

void CDpllAssignment::assign(const int lit)
{
    if (match_.value_vec[std::abs(lit) - 1u] != SMatch::EValue::NONE)
        throw CException("error: reassigning literal");

    match_.value_vec[std::abs(lit) - 1u] = 
        (lit < 0 ? SMatch::EValue::FALSE : SMatch::EValue::TRUE);

    sort_heap_.extract(lit);
    sort_heap_.extract(-lit);
}

void CDpllAssignment::revert(const int lit)
{
    if (match_.value_vec[std::abs(lit) - 1u] == SMatch::EValue::NONE)
        throw CException("error: reverting not assigned literal");

    sort_heap_.restore(-lit);
    sort_heap_.restore(lit);

    sort_heap_.dec_prior(lit);

    match_.value_vec[std::abs(lit) - 1u] = SMatch::EValue::NONE;
}

} // namespace tinysat

#endif // TINYSAT_DPLL_CDPLLASSIGNMENT_HPP_
