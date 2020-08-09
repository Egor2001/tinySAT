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

    [[nodiscard]] friend 
    bool operator == (const CDpllContext&, const CDpllContext&);
    [[nodiscard]] friend 
    bool operator != (const CDpllContext&, const CDpllContext&);

protected:
    [[nodiscard]] bool search();
    [[nodiscard]] bool propagate(int);

private:
    CDpllAssignment assignment_;
    CDpllFormula formula_;

    std::stack<SState> state_stack_;
};

} // namespace tinysat

#endif // TINYSAT_DPLL_CDPLLCONTEXT_HPP_
