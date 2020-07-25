#include "CBinarySolver.hpp"

namespace tinysat {

CBinarySolver::CBinarySolver(const SFormula& formula)
{
    reset(formula);
}

CBinarySolver::CBinarySolver(SFormula&& formula)
{
    reset(std::move(formula));
}

void CBinarySolver::reset(const SFormula& formula)
{
    formula_ = formula;
    init();
}

void CBinarySolver::reset(SFormula&& formula)
{
    formula_ = std::move(formula);
    init();
}

std::unique_ptr<CBinarySolver::CContext>
CBinarySolver::context() const
{
    return nullptr;
}

void CBinarySolver::proceed(std::unique_ptr<CContext>& context) const
{
    [[unlikely]]
    if (context == nullptr)
        throw CException("trying to proceed over the end");

    context = nullptr;
}

CBinarySolver::CIterator CBinarySolver::begin() const
{
    return CIterator(this, context());
}

CBinarySolver::CIterator CBinarySolver::end() const
{
    return CIterator(this, nullptr);
}

void CBinarySolver::init()
{
    for (const auto& clause : formula_.clause_vec)
    {
        [[unlikely]]
        if (clause.size() > 2u)
            throw CException("2-SAT solver initialized with non-2-SAT");
    }
}

/*
    SFormula formula_;

    std::vector<size_t> comp_vec_;
    std::vector<std::vector<size_t>> cond_edge_vec_;
*/

SMatch 
CBinarySolver::CContext::
match() const
{
    return SMatch{};
}

CBinarySolver::CContext::
CContext(const std::vector<size_t>& order_vec):
    order_vec_(order_vec)
{}

CBinarySolver::CContext::
CContext(std::vector<size_t>&& order_vec):
    order_vec_(std::move(order_vec))
{}

bool operator == (const CBinarySolver::CContext& lhs,
                  const CBinarySolver::CContext& rhs)
{
    return lhs.order_vec_ == rhs.order_vec_;
}

bool operator != (const CBinarySolver::CContext& lhs,
                  const CBinarySolver::CContext& rhs)
{
    return !(lhs == rhs);
}

/*
    std::vector<size_t> order_vec_;
*/

} // namespace tinysat 
