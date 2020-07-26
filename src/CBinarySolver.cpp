#include "CBinarySolver.hpp"

namespace tinysat {

CBinarySolver::CBinarySolver(const SFormula& formula):
    formula_{},
    comp_vec_{},
    comp_graph_(2u*formula.params_cnt)
{
    reset(formula);
}

CBinarySolver::CBinarySolver(SFormula&& formula):
    formula_{},
    comp_vec_{},
    comp_graph_(2u*formula.params_cnt)
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
    for (size_t param = 0u; param < formula_.params_cnt; ++param)
    {
        if (comp_vec_[param] == comp_vec_[param + formula_.params_cnt])
            return nullptr;
    }

    size_t comp_cnt = comp_graph_.edge_vec().size();

    std::vector<size_t> order_vec(comp_cnt);
    for (size_t comp = 0u; comp < comp_cnt; ++comp)
        order_vec[comp] = comp;

    auto result = std::make_unique<CContext>(std::move(order_vec));
    set_match(*result);

    return std::move(result);
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

    size_t half_cnt = formula_.params_cnt;
    CGraph<std::set> impl_graph(2u*half_cnt);

    for (const auto& clause : formula_.clause_vec)
    {
        [[unlikely]]
        if (clause.empty())
            continue;

        int lhs_v = clause[0u];
        int rhs_v = (clause.size() == 2u ? clause[1u] : lhs_v);

        lhs_v = (lhs_v > 0 ? (lhs_v - 1) : half_cnt + (-lhs_v - 1));
        rhs_v = (rhs_v > 0 ? (rhs_v - 1) : half_cnt + (-rhs_v - 1));

        impl_graph.insert_or_edge((lhs_v + half_cnt)%(2u*half_cnt), rhs_v);
        impl_graph.insert_or_edge((rhs_v + half_cnt)%(2u*half_cnt), lhs_v);
    }

    comp_vec_ = impl_graph.decompose();
    comp_graph_ = impl_graph.compress(comp_vec_);
}

void CBinarySolver::set_match(CContext& context) const
{
    context.match_.value_vec.resize(formula_.params_cnt);

    for (size_t param = 0u; param < formula_.params_cnt; ++param)
    {
        context.match_.value_vec[param] =
            (context.order_vec_[comp_vec_[param]] < 
             context.order_vec_[comp_vec_[param + formula_.params_cnt]] ?
             SMatch::EValue::FALSE : SMatch::EValue::TRUE);
    }
}

/*
    SFormula formula_;

    std::vector<size_t> comp_vec_;
    CGraph<std::set> comp_graph_;
*/

CBinarySolver::CContext::
CContext(const std::vector<size_t>& order_vec):
    match_{},
    order_vec_(order_vec)
{}

CBinarySolver::CContext::
CContext(std::vector<size_t>&& order_vec):
    match_{},
    order_vec_(std::move(order_vec))
{}

SMatch 
CBinarySolver::CContext::
match() const
{
    return match_;
}

bool operator == (const CBinarySolver::CContext& lhs,
                  const CBinarySolver::CContext& rhs)
{
    return (lhs.match_ == rhs.match_) && (lhs.order_vec_ == rhs.order_vec_);
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
