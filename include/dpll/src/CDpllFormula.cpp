#include "CDpllFormula.hpp"

namespace tinysat {

CDpllFormula::CDpllFormula(const SFormula& formula)
{
    const size_t clauses_cnt = formula.clause_vec.size();
    for (size_t idx = 0u; idx < clauses_cnt; ++idx)
    {
        clauses_.push_back(SDpllClause { 
                .idx = idx, 
                .literals = SDpllClause::TContainer(
                        formula.clause_vec[idx]) 
            });

        iter_map_.push_back(std::prev(std::end(clauses_)));
    }
}

CDpllFormula::CDpllFormula(SFormula&& formula)
{
    const size_t clauses_cnt = formula.clause_vec.size();
    for (size_t idx = 0u; idx < clauses_cnt; ++idx)
    {
        clauses_.push_back(SDpllClause { 
                .idx = idx, 
                .literals = SDpllClause::TContainer(
                        std::move(formula.clause_vec[idx])) 
            });

        iter_map_.push_back(std::prev(std::end(clauses_)));
    }
}

CDpllFormula::SState 
CDpllFormula::get_state() const
{
    return SState { 
        .lit_log_idx = lit_log_stk_.size(),
        .cls_log_idx = cls_log_stk_.size()
    };
}

void CDpllFormula::backtrack(const SState& state)
{
    while (state.cls_log_idx < cls_log_stk_.size())
    {
        clauses_.restore(std::move(cls_log_stk_.top()));
        cls_log_stk_.pop();
    }

    while (state.lit_log_idx < lit_log_stk_.size())
    {
        iter_map_[lit_log_stk_.top().idx]->literals
            .restore(std::move(lit_log_stk_.top().node));
        lit_log_stk_.pop();
    }
}

// TODO:
// - to store current assignment
// - to check for lit to be already assigned
// - to eliminate unit clauses in-place
bool CDpllFormula::proceed(const int lit)
{
    bool result = true;

    stats_.unary_clause_set.erase(lit);

    auto cls_it = std::begin(clauses_);
    while (cls_it != std::end(clauses_))
    {
        auto lit_end = std::end(cls_it->literals);
        auto lit_it = lit_end;

        if ((lit_it = cls_it->literals.find(lit)) != lit_end)
        {
            ++cls_it;
            cls_log_stk_.push(clauses_.extract(std::prev(cls_it)));
            continue;
        }
        else if ((lit_it = cls_it->literals.find(-lit)) != lit_end)
        {
            ++lit_it;
            lit_log_stk_.push(SLitLogEntry { 
                .idx = cls_it->idx, 
                .node = cls_it->literals.extract(std::prev(lit_it))
            });
        }

        if (cls_it->literals.size() == 0u)
        {
            result = false;
            break;
        }
        else if (cls_it->literals.size() == 1u)
        {
            stats_.unary_clause_set.insert(*std::begin(cls_it->literals));
        }

        ++cls_it;
    }

    return result;
}

} // namespace tinysat
