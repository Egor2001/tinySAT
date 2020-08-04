#include "CDavisPutnamSolver.hpp"

namespace tinysat {

CDavisPutnamSolver::CDavisPutnamSolver(const SFormula& formula)
{
    reset(formula);
}

CDavisPutnamSolver::CDavisPutnamSolver(SFormula&& formula)
{
    reset(std::move(formula));
}

CDavisPutnamSolver::reset(const SFormula& formula)
{
    formula_ = formula;
    init();
}

CDavisPutnamSolver::reset(SFormula&& formula)
{
    formula_ = std::move(formula);
    init();
}

std::unique_ptr<CDavisPutnamSolver::CContext> 
CDavisPutnamSolver::
context() const
{
    return nullptr;
}

void 
CDavisPutnamSolver::
proceed(std::unique_ptr<CContext>& context) const
{
    [[unlikely]]
    if (context == nullptr)
        throw CException("trying to proceed over the end");

    return nullptr;
}

CDavisPutnamSolver::CIterator 
CDavisPutnamSolver::
begin() const
{
    return CIterator(this, context());
}

CDavisPutnamSolver::CIterator 
CDavisPutnamSolver::
end() const
{
    return CIterator(this, nullptr);
}

void 
CDavisPutnamSolver::
init()
{
}

/*
    SFormula formula_;
*/

SMatch 
CDavisPutnamSolver::CContext
match() const
{
    return match_;
}

/*
    SMatch match_;
*/

} // namespace tinysat
