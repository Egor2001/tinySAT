#ifndef TINYSAT_DPLL_CDPLLCONTEXT_HPP_
#define TINYSAT_DPLL_CDPLLCONTEXT_HPP_

#include "CDpllFormula.hpp"
#include "CDpllAssignment.hpp"

namespace tinysat {

class CDpllContext
{
public:
    using TAssignmentState = CDpllAssignment::SState;
    using TFormulaState = CDpllFormula::SState;

    struct SState
    {
        int lit;
        SMatch::EValue val;
        TAssignmentState assignment_state;
        TFormulaState formula_state;
    };

    CDpllContext() = default;
    explicit CDpllContext(const SFormula&);
    explicit CDpllContext(SFormula&&);

    CDpllContext             (const CDpllContext&) = default;
    CDpllContext& operator = (const CDpllContext&) = default;
    CDpllContext             (CDpllContext&&) = default;
    CDpllContext& operator = (CDpllContext&&) = default;

    [[nodiscard]] const SMatch& match() const
    {
        return assignment_.match();
    }

    [[nodiscard]] const CDpllFormula::SStats& stats() const
    {
        return formula_.stats();
    }

    [[nodiscard]] bool init();
    [[nodiscard]] bool next();

protected:
    [[nodiscard]] bool search();
    [[nodiscard]] bool propagate(int);

private:
    CDpllAssignment assignment_;
    CDpllFormula formula_;

    std::stack<SState> state_stack_;
};

CDpllContext::CDpllContext(const SFormula& formula):
    assignment_(formula),
    formula_(formula)
{}

CDpllContext::CDpllContext(SFormula&& formula):
    assignment_(formula),
    formula_(std::move(formula))
{}

bool CDpllContext::init()
{
    state_stack_.push(SState {
        .lit = assignment_.request(),
        .val = SMatch::EValue::TRUE,
        .assignment_state = assignment_.get_state(),
        .formula_state = formula_.get_state()
    });

    return search();
}

bool CDpllContext::next()
{
    if (!state_stack_.empty())
    {
        auto& top = state_stack_.top();

        top.val = (top.val == SMatch::EValue::TRUE ? 
                   SMatch::EValue::FALSE : SMatch::EValue::NONE);

        assignment_.backtrack(top.assignment_state);
        formula_.backtrack(top.formula_state);
    }

    return (state_stack_.empty() ? false : search());
}

bool CDpllContext::search()
{
    while (!state_stack_.empty())
    {
        auto& top = state_stack_.top();

        if (top.lit == 0)
        {
            state_stack_.pop();
            break;
        }

        if (top.val == SMatch::EValue::NONE)
        {
            assignment_.backtrack(top.assignment_state);
            formula_.backtrack(top.formula_state);

            state_stack_.pop();
            continue;
        }

        auto prop_lit = 0;
        if (top.val == SMatch::EValue::TRUE)
        {
            prop_lit = top.lit;
            top.val = SMatch::EValue::FALSE;
        }
        else
        {
            prop_lit = -top.lit;
            top.val = SMatch::EValue::NONE;
        }

        if (propagate(prop_lit))
        {
            state_stack_.push(SState {
                .lit = assignment_.request(),
                .val = SMatch::EValue::TRUE,
                .assignment_state = assignment_.get_state(),
                .formula_state = formula_.get_state()
            });
        }
        else
        {
            assignment_.backtrack(top.assignment_state);
            formula_.backtrack(top.formula_state);
        }
    }

    return !state_stack_.empty();
}

bool CDpllContext::propagate(int lit)
{
    while (lit != 0 && formula_.proceed(lit))
    {
        assignment_.proceed(lit);
        lit = 0;

        if (auto prop_it = std::begin(stats().unary_clause_set); 
            prop_it != std::end(stats().unary_clause_set))
        {
            lit = *prop_it;
        }
    }

    return (lit == 0);
}

} // namespace tinysat

#endif // TINYSAT_DPLL_CDPLLCONTEXT_HPP_
