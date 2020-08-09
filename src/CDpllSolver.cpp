#include "CDpllSolver.hpp"

namespace tinysat {

CDpllSolver::CContext::CContext(const SFormula& formula):
    dpll_context_(formula)
{}

SMatch CDpllSolver::CContext::match() const
{
    return dpll_context_.match();
}

CDpllSolver::CDpllSolver(const SFormula& formula):
    formula_(formula)
{}

CDpllSolver::CDpllSolver(SFormula&& formula):
    formula_(std::move(formula))
{}

std::unique_ptr<CDpllSolver::CContext> CDpllSolver::context() const
{
    auto context = std::make_unique<CContext>(formula_);
    if (context->dpll_context_.init() == false)
        context.reset();

    return context;
}

void CDpllSolver::proceed(std::unique_ptr<CContext>& context) const
{
    if (context == nullptr)
        throw CException("trying to proceed over the end");

    if (context->dpll_context_.next() == false)
        context.reset();
}

CDpllSolver::CIterator CDpllSolver::begin() const
{
    return CIterator(this, context());
}

CDpllSolver::CIterator CDpllSolver::end() const
{
    return CIterator(this, nullptr);
}

bool operator == (const CDpllSolver::CContext& lhs,
                  const CDpllSolver::CContext& rhs)
{
    return lhs.dpll_context_ == rhs.dpll_context_;
}

bool operator != (const CDpllSolver::CContext& lhs,
                  const CDpllSolver::CContext& rhs)
{
    return !(lhs == rhs);
}

} // namespace tinysat
