#ifndef TINYSAT_DPLL_CDPLLCONTEXT_HPP_
#define TINYSAT_DPLL_CDPLLCONTEXT_HPP_

#include <algorithm>
#include <vector>
#include <stack>
#include <list>
#include <unordered_set>

#include "SFormula.hpp"
#include "../src/SFormula.cpp"
#include "misc/CBacktrackSkiplist.hpp"
#include "misc/CBacktrackList.hpp"

namespace tinysat {

template<typename TData>
class CChangeLog
{
public:
    struct SEntry
    {
        size_t level;
        TData data;
    };

    [[nodiscard]] bool empty() const noexcept
    {
        return entry_stk_.empty();
    }

    void push(const TData& data, size_t level)
    {
        entry_stk_.push(SEntry{ .level = level, .data = data });
    }

    void push(TData&& data, size_t level)
    {
        entry_stk_.push(SEntry{ .level = level, .data = std::move(data) });
    }

    void pop()
    {
        entry_stk_.pop();
    }

    [[nodiscard]] size_t level() const
    {
        return (empty() ? 0 : entry_stk_.top().level);
    }

    [[nodiscard]] TData& top()
    {
        return entry_stk_.top().data;
    }

    [[nodiscard]] const TData& top() const
    {
        return entry_stk_.top().data;
    }

private:
    std::stack<SEntry> entry_stk_;
};

struct SDpllClause
{
public:
    using TContainer = CBacktrackSkiplist<int>;
    using TIter = TContainer::iterator;
    using TNode = TContainer::node_type;

    size_t idx;
    TContainer literals;
};

struct SDpllFormula
{
public:
    using TContainer = CBacktrackList<SDpllClause>;
    using TIter = TContainer::iterator;
    using TNode = TContainer::node_type;

    struct SState
    {
        size_t level;
    };

    struct SBacktrackResult
    {
    };

    struct SPropagateResult
    {
        bool conflict;
        std::list<int> lit_list;
    };

    explicit SDpllFormula(const SFormula& formula);
    explicit SDpllFormula(SFormula&& formula);

    SDpllFormula             (const SDpllFormula&) = default;
    SDpllFormula& operator = (const SDpllFormula&) = default;
    SDpllFormula             (SDpllFormula&&) = default;
    SDpllFormula& operator = (SDpllFormula&&) = default;

    SState get_state() const;
    SPropagateResult propagate(int lit);
    SBacktrackResult backtrack(const SState& state);

protected:
    struct SLitLogEntry
    {
        size_t idx;
        SDpllClause::TNode node;
    };

    using TClsLogEntry = TNode;

private:
    size_t cur_level_ = 0u;

    std::vector<TIter> iter_map_;
    TContainer clauses_;

    CChangeLog<SLitLogEntry> lit_log_;
    CChangeLog<TClsLogEntry> cls_log_;
};

SDpllFormula::SDpllFormula(const SFormula& formula)
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

SDpllFormula::SDpllFormula(SFormula&& formula)
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

SDpllFormula::SState 
SDpllFormula::get_state() const
{
    return SState { .level = cur_level_ };
}

// TODO:
// - to store current assignment
// - to check for lit to be already assigned
// - to eliminate unit clauses in-place
SDpllFormula::SPropagateResult 
SDpllFormula::propagate(int lit)
{
    SPropagateResult result = {
        .conflict = false,
        .lit_list = {}
    };

    std::unordered_set<int> prop_set;
    prop_set.insert(lit);

    while (!prop_set.empty())
    {
        ++cur_level_;

        auto prop_it = std::begin(prop_set);
        lit = *prop_it;
        prop_set.erase(prop_it);

        result.lit_list.push_back(lit);

        auto cls_it = std::begin(clauses_);
        while (cls_it != std::end(clauses_))
        {
            auto end = std::end(cls_it->literals);
            auto lit_it = end;
            if ((lit_it = cls_it->literals.find(lit)) != end)
            {
                ++cls_it;
                cls_log_.push(clauses_.extract(std::prev(cls_it)), cur_level_);

                continue;
            }
            else if ((lit_it = cls_it->literals.find(-lit)) != end)
            {
                SLitLogEntry entry = { 
                    .idx = cls_it->idx, 
                    .node = cls_it->literals.extract(lit_it)
                };

                lit_log_.push(std::move(entry), cur_level_);
            }

            if (cls_it->literals.size() == 0u)
            {
                result.conflict = true;
                return result;
            }
            else if (cls_it->literals.size() == 1u)
            {
                prop_set.insert(*std::begin(cls_it->literals));
            }

            ++cls_it;
        }
    }

    result.conflict = false;
    return result;
}

SDpllFormula::SBacktrackResult
SDpllFormula::backtrack(const SState& state)
{
    while (!cls_log_.empty() && state.level < cls_log_.level())
    {
        clauses_.restore(std::move(cls_log_.top()));
        cls_log_.pop();
    }

    while (!lit_log_.empty() && state.level < lit_log_.level())
    {
        iter_map_[lit_log_.top().idx]->literals
            .restore(std::move(lit_log_.top().node));
        lit_log_.pop();
    }

    cur_level_ = state.level;

    return SBacktrackResult{};
}

} // namespace tinysat

#endif // TINYSAT_DPLL_CDPLLCONTEXT_HPP_
