#ifndef TINYSAT_CDPLLSOLVER_HPP_
#define TINYSAT_CDPLLSOLVER_HPP_

#include <memory>

#include "misc/CBacktrackSkiplist.hpp"
#include "CMatchIterator.hpp"
#include "SFormula.hpp"
#include "SMatch.hpp"

namespace tinysat {

class CDpllSolver
{
public:
    struct SContext;

    using context_t = SContext;
    using CIterator = CMatchIterator<const CDpllSolver>;

    explicit CDpllSolver(const SFormula&);
    explicit CDpllSolver(SFormula&&);

    CDpllSolver             (const CDpllSolver&) = default;
    CDpllSolver& operator = (const CDpllSolver&) = default;
    CDpllSolver             (CDpllSolver&&) = default;
    CDpllSolver& operator = (CDpllSolver&&) = default;

    [[nodiscard]] std::unique_ptr<context_t> context() const;
    void proceed(std::unique_ptr<context_t>&) const;

    [[nodiscard]] CIterator begin() const;
    [[nodiscard]] CIterator end() const;

private:
    using TClause = CBacktrackSkiplist<int>;
    using TClauseList = CBacktrackList<TClause>;

    using TClauseIter = decltype(std::begin(std::declval<TClause>()));
    using TClauseListIter = decltype(std::begin(std::declval<TClauseList>()));

    SFormula formula_;

    TClauseList clause_mtx_; 
    std::
};

struct CDpllSolver::SContext
{
public:
    using TClause = CBacktrackSkiplist<int>;
    using TClauseList = CList<TClauseList>;

    using TClauseIter = decltype(std::begin(std::declval<TClause>()));
    using TClauseListIter = decltype(std::begin(std::declval<TClauseList>()));

    struct SState
    {
        std::stack<std::pair<size_t, int>> prop_stack_;
        std::stack<std::pair<size_t, TClauseIter>> lit_stack;
        std::stack<std::pair<size_t, TClauseListIter>> cls_stack;
    };

    SMatch match() const;

private:
    SMatch match_;

    TClauseList clause_list_;

    std::stack<SState> state_stack_;
};

} // namespace tinysat

#endif // TINYSAT_CDPLLSOLVER_HPP_
