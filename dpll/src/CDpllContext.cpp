#include "CDpllContext.hpp"

/**
 * @file CDpllContext.cpp
 * @author geome_try
 * @date 2020
 * @see CDpllContext.hpp
 */

/// @brief
namespace tinysat {

/**
 * @param [in] formula SAT formula to build context from via copy
 */
CDpllContext::CDpllContext(const SFormula& formula):
    assignment_(formula),
    formula_(formula)
{}

/**
 * @param [in,out] formula SAT formula to build context from via move
 */
CDpllContext::CDpllContext(SFormula&& formula):
    assignment_(formula),
    formula_(std::move(formula))
{}

/**
 * @return true if any solution exists
 * @see next()
 * @see search()
 */
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

/**
 * @return true if next solution exists
 * @see init()
 * @see search()
 */
bool CDpllContext::next()
{
    if (!state_stack_.empty())
    {
        auto& top = state_stack_.top();

        assignment_.backtrack(top.assignment_state);
        formula_.backtrack(top.formula_state);
    }

    return search();
}

/**
 * @return true if search succedes
 * @see init()
 * @see next()
 */
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

/**
 * @return true if propagation doesn't imply conflicts
 * @see proceed()
 */
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

/**
 * @param [in] lhs
 * @param [in] rhs
 * @return true if lhs and rhs are same objects
 * @see operator!=()
 */
bool operator == (const CDpllContext& lhs, const CDpllContext& rhs)
{
    return &lhs == &rhs;
}

/**
 * @param [in] lhs
 * @param [in] rhs
 * @return true if lhs and rhs aren't same objects
 * @see operator==()
 */
bool operator != (const CDpllContext& lhs, const CDpllContext& rhs)
{
    return !(lhs == rhs);
}

} // namespace tinysat
