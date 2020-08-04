#ifndef TINYSAT_CDPLLSOLVER_HPP_
#define TINYSAT_CDPLLSOLVER_HPP_

#include <memory>

#include "CException.hpp"
#include "CMatchIterator.hpp"
#include "SFormula.hpp"
#include "SMatch.hpp"

#include "dpll/CDpllFormula.hpp"
#include "dpll/CDpllContext.hpp"

namespace tinysat {

class CDpllSolver
{
public:
    class CContext;

    using context_t = CContext;
    using CIterator = CMatchIterator<const CDpllSolver>;

    explicit CDpllSolver(const SFormula&);
    explicit CDpllSolver(SFormula&&);

    CDpllSolver             (const CDpllSolver&) = default;
    CDpllSolver& operator = (const CDpllSolver&) = default;
    CDpllSolver             (CDpllSolver&&) = default;
    CDpllSolver& operator = (CDpllSolver&&) = default;

    [[nodiscard]] std::unique_ptr<context_t> context() const;
    void proceed(std::unique_ptr<context_t>&) const;

    [[nodiscard]] CIterator begin() const;
    [[nodiscard]] CIterator end() const;

private:
    SFormula formula_;
};

struct CDpllSolver::CContext
{
public:
    explicit CContext(const SFormula&);

    CContext             (const CContext&) = default;
    CContext& operator = (const CContext&) = default;
    CContext             (CContext&&) = default;
    CContext& operator = (CContext&&) = default;

    [[nodiscard]] SMatch match() const;

protected:
    bool solve();

private:
    CDpllContext dpll_context_;
    CDpllFormula dpll_formula_;
};

CDpllSolver::CContext::CContext(const SFormula& formula):
    dpll_context_(formula),
    dpll_formula_(formula)
{}

SMatch CDpllSolver::CContext::match() const
{
    return dpll_context_.match();
}

bool CDpllSolver::CContext::solve()
{
    bool result = false;
    while (!state_stack_.empty() && 
           dpll_context_.size() < dpll_context_.match().value_vec.size())
    {
        auto lit = dpll_context_.request();
        auto cur = dpll_formula_.get_state();
        auto res = dpll_formula_.propagate(lit);

        if (res.conflict)
        {
            dpll_formula_.backtrack(cur);
        }
        else
        {
            for (const auto& elem : res.lit_list)
                dpll_context_.assign(elem);
        }
    }

    return result;
}

CDpllSolver::CDpllSolver(const SFormula& formula):
    formula_(formula)
{}

CDpllSolver::CDpllSolver(SFormula&& formula):
    formula_(std::move(formula))
{}

std::unique_ptr<CDpllSolver::CContext> CDpllSolver::context() const
{
    return nullptr;
}

void CDpllSolver::proceed(std::unique_ptr<CContext>& context) const
{
    if (context == nullptr)
        throw CException("trying to proceed over the end");

    context.reset(nullptr);
}

CDpllSolver::CIterator CDpllSolver::begin() const
{
    return CIterator(this, context());
}

CDpllSolver::CIterator CDpllSolver::end() const
{
    return CIterator(this, nullptr);
}

} // namespace tinysat

#endif // TINYSAT_CDPLLSOLVER_HPP_
