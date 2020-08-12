#ifndef TINYSAT_DPLL_CDPLLFORMULA_HPP_
#define TINYSAT_DPLL_CDPLLFORMULA_HPP_

#include <algorithm>
#include <vector>
#include <stack>
#include <unordered_set>

#include "SFormula.hpp"

#include "misc/CBacktrackSkiplist.hpp"
#include "misc/CBacktrackList.hpp"

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

} // namespace tinysat

#endif // TINYSAT_DPLL_CDPLLFORMULA_HPP_
