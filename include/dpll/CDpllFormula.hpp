#ifndef TINYSAT_DPLL_CDPLLFORMULA_HPP_
#define TINYSAT_DPLL_CDPLLFORMULA_HPP_

#include <algorithm>
#include <vector>
#include <stack>
#include <list>
#include <unordered_set>

#include "SFormula.hpp"
//#include "../src/SFormula.cpp"

#include "misc/CBacktrackSkiplist.hpp"
#include "misc/CBacktrackList.hpp"
#include "misc/CChangeLog.hpp"

namespace tinysat {

struct SDpllClause
{
public:
    using TContainer = CBacktrackSkiplist<int>;
    using TIter = TContainer::iterator;
    using TNode = TContainer::node_type;

    size_t idx;
    TContainer literals;
};

class CDpllFormula
{
public:
    using TContainer = CBacktrackList<SDpllClause>;
    using TIter = TContainer::iterator;
    using TNode = TContainer::node_type;

    struct SStats
    {
        std::unordered_set<int> unary_clause_set;
    };

    struct SState
    {
        size_t lit_log_idx;
        size_t cls_log_idx;
    };

    explicit CDpllFormula(const SFormula&);
    explicit CDpllFormula(SFormula&&);

    CDpllFormula             (const CDpllFormula&) = default;
    CDpllFormula& operator = (const CDpllFormula&) = default;
    CDpllFormula             (CDpllFormula&&) = default;
    CDpllFormula& operator = (CDpllFormula&&) = default;

    [[nodiscard]] const SStats& stats() const noexcept
    {
        return stats_;
    }

    SState get_state() const;
    void backtrack(const SState&);
    bool proceed(const int);

protected:
    struct SLitLogEntry
    {
        size_t idx;
        SDpllClause::TNode node;
    };

    using TClsLogEntry = TNode;

private:
    SStats stats_;

    std::vector<TIter> iter_map_;
    TContainer clauses_;

    std::stack<SLitLogEntry> lit_log_stk_;
    std::stack<TClsLogEntry> cls_log_stk_;
};

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

#endif // TINYSAT_DPLL_CDPLLFORMULA_HPP_
