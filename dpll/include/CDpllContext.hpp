#ifndef TINYSAT_DPLL_CDPLLCONTEXT_HPP_
#define TINYSAT_DPLL_CDPLLCONTEXT_HPP_

/**
 * @file CDpllContext.hpp
 * @author geome_try
 * @date 2020
 */

#include "CDpllFormula.hpp"
#include "CDpllAssignment.hpp"

/// @brief
namespace tinysat {

/// Represents context for the DPLL algorithm
class CDpllContext;

/**
 * Holds SAT formula and assignment for the DPLL algorithm
 */
class CDpllContext
{
public:
    /// State class for the assignment
    using TAssignmentState = CDpllAssignment::SState;
    /// State class for the formula
    using TFormulaState = CDpllFormula::SState;

    /// Represents state of the decision tree
    struct SState
    {
        int lit; ///< Branching literal
        SMatch::EValue val; ///< Current literal's value
        TAssignmentState assignment_state; /// Assignment's state
        TFormulaState formula_state; /// Formula's state
    };

    /// Default ctor
    CDpllContext() = default;

    explicit CDpllContext(const SFormula&); ///< Copy ctor from the SAT formula
    explicit CDpllContext(SFormula&&); ///< Move ctor from the SAT formula

    CDpllContext             (const CDpllContext&) = default; ///< Rule of 5
    CDpllContext& operator = (const CDpllContext&) = default; ///< Rule of 5
    CDpllContext             (CDpllContext&&) = default; ///< Rule of 5
    CDpllContext& operator = (CDpllContext&&) = default; ///< Rule of 5

    /// Returns DPLL match
    /**
     * @return assignment's match 
     */
    [[nodiscard]] const SMatch& match() const
    {
        return assignment_.match();
    }

    /// Returns DPLL stats
    /**
     * @return formula's statistics 
     */
    [[nodiscard]] const CDpllFormula::SStats& stats() const
    {
        return formula_.stats();
    }

    [[nodiscard]] bool init(); ///< Find an initial solution
    [[nodiscard]] bool next(); ///< Find the next solution

    /// Compares DPLL contexts for the equality
    [[nodiscard]] friend 
    bool operator == (const CDpllContext&, const CDpllContext&);

    /// Compares DPLL contexts for the inequality
    [[nodiscard]] friend 
    bool operator != (const CDpllContext&, const CDpllContext&);

protected:
    /// Search for the next decision tree leaf case
    [[nodiscard]] bool search();

    /// Propagate the given literal
    [[nodiscard]] bool propagate(int);

private:
    CDpllAssignment assignment_;
    CDpllFormula formula_;

    std::stack<SState> state_stack_;
};

} // namespace tinysat

#endif // TINYSAT_DPLL_CDPLLCONTEXT_HPP_
