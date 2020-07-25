#include "CGeneralSolver.hpp"

namespace tinysat {

CGeneralSolver::CGeneralSolver(const SFormula& formula)
{
    reset(formula);
}

CGeneralSolver::CGeneralSolver(SFormula&& formula)
{
    reset(std::move(formula));
}

void CGeneralSolver::reset(const SFormula& formula)
{
    formula_ = formula;
    init();
}

void CGeneralSolver::reset(SFormula&& formula)
{
    formula_ = std::move(formula);
    init();
}

std::unique_ptr<CGeneralSolver::CContext> CGeneralSolver::context() const
{
    SMatch match = {
        .value_vec = std::vector<SMatch::EValue>(formula_.params_cnt, 
                                                 SMatch::EValue::TRUE)
    };

    auto result = std::make_unique<CContext>(std::move(match));

    if (!formula_.is_match(result->match()) && !propagate(*result))
        result.reset();

    return std::move(result);
}

void CGeneralSolver::proceed(std::unique_ptr<CContext>& context) const
{
    [[unlikely]]
    if (context == nullptr)
        throw CException("trying to proceed over the end");

    if (!propagate(*context))
        context.reset();
}

CGeneralSolver::CIterator CGeneralSolver::begin() const
{
    return CIterator(this, context());
}

CGeneralSolver::CIterator CGeneralSolver::end() const
{
    return CIterator(this, nullptr);
}

void CGeneralSolver::init()
{
}

bool CGeneralSolver::propagate(CContext& context) const
{
    do
    {
        size_t idx = 0u;
        while (idx < formula_.params_cnt && 
               context.match_.value_vec[idx] == SMatch::EValue::FALSE)
        {
            context.match_.value_vec[idx] = SMatch::EValue::TRUE;
            ++idx;
        }

        if (idx == formula_.params_cnt)
            return false;

        context.match_.value_vec[idx] = SMatch::EValue::FALSE;
    }
    while (!formula_.is_match(context.match_));

    return true;
}

/*
    SFormula formula_;
*/

CGeneralSolver::CContext::
CContext(const SMatch& match):
    match_(match)
{}

CGeneralSolver::CContext::
CContext(SMatch&& match):
    match_(std::move(match))
{}

SMatch 
CGeneralSolver::CContext::
match() const
{
    return match_;
}

bool operator == (const CGeneralSolver::CContext& lhs,
                  const CGeneralSolver::CContext& rhs)
{
    return lhs.match_ == rhs.match_;
}

bool operator != (const CGeneralSolver::CContext& lhs,
                  const CGeneralSolver::CContext& rhs)
{
    return !(lhs == rhs);
}

/*
    SMatch match_;
*/

} // namespace tinysat
