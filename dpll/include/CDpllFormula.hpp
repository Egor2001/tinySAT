#ifndef TINYSAT_DPLL_CDPLLFORMULA_HPP_
#define TINYSAT_DPLL_CDPLLFORMULA_HPP_

/**
 * @file CDpllFormula.hpp
 * @author geome_try
 * @date 2020
 */

#include <algorithm>
#include <vector>
#include <stack>
#include <unordered_set>

#include "SFormula.hpp"

#include "misc/CBacktrackSkiplist.hpp"
#include "misc/CBacktrackList.hpp"

/// @brief
namespace tinysat {

/// SAT clause representation for the DPLL algorithm
struct SDpllClause;

/// SAT formula representation for the DPLL algorithm
class CDpllFormula;

/**
 * Represents clause as list of literals
 */
struct SDpllClause
{
public:
    using TContainer = CBacktrackSkiplist<int>; ///< literal container's type
    using TIter = TContainer::iterator; ///< container's iterator
    using TNode = TContainer::node_type; ///< container's node type

    size_t idx; ///< index of the clause
    TContainer literals; ///< clause literals' list
};

/**
 * Represents formula as list of literals
 * and holds additional info such as stats and stacks for backtracking
 */
class CDpllFormula
{
public:
    using TContainer = CBacktrackList<SDpllClause>; ///< clause container's type
    using TIter = TContainer::iterator; ///< container's iterator
    using TNode = TContainer::node_type; ///< container's node type

    /// Holds stats for the DPLL's heuristics
    struct SStats
    {
        /// Set of literals with unit clauses
        std::unordered_set<int> unary_clause_set;
    };

    /// Holds state for backtracking
    struct SState
    {
        size_t lit_log_idx; ///< size of the literal log stack
        size_t cls_log_idx; ///< size of the clause log stack
    };

    /// Copy ctor from the SAT formula
    explicit CDpllFormula(const SFormula&);
    /// Move ctor from the SAT formula
    explicit CDpllFormula(SFormula&&);

    CDpllFormula             (const CDpllFormula&) = default; ///< Rule of 5
    CDpllFormula& operator = (const CDpllFormula&) = default; ///< Rule of 5
    CDpllFormula             (CDpllFormula&&) = default; ///< Rule of 5
    CDpllFormula& operator = (CDpllFormula&&) = default; ///< Rule of 5

    /// Returns current statistics
    [[nodiscard]] const SStats& stats() const noexcept;

    /// Returns current state
    [[nodiscard]] SState get_state() const noexcept;

    /// Backtrack to the given state
    void backtrack(const SState&);

    /// Assign true to the given literal
    bool proceed(const int);

protected:
    /// Represents single extracted literal
    /**
     * @see TClsLogEntry
     */
    struct SLitLogEntry
    {
        size_t idx;
        SDpllClause::TNode node;
    };

    /// Represents single extracted clause
    /**
     * @see SLitLogEntry
     */
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
