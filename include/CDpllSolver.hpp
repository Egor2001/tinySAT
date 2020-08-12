#ifndef TINYSAT_CDPLLSOLVER_HPP_
#define TINYSAT_CDPLLSOLVER_HPP_

#include <memory>

#include "CException.hpp"
#include "CMatchIterator.hpp"
#include "SFormula.hpp"
#include "SMatch.hpp"

#include "include/CDpllContext.hpp"

namespace tinysat {

class CDpllSolver
{
public:
    class CContext;

    using context_t = CContext;
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
    SFormula formula_;
};

struct CDpllSolver::CContext
{
public:
    friend class CDpllSolver;

    explicit CContext(const SFormula&);

    CContext             (const CContext&) = default;
    CContext& operator = (const CContext&) = default;
    CContext             (CContext&&) = default;
    CContext& operator = (CContext&&) = default;

    [[nodiscard]] SMatch match() const;

    [[nodiscard]] friend 
    bool operator == (const CContext& lhs, const CContext& rhs);
    [[nodiscard]] friend 
    bool operator != (const CContext& lhs, const CContext& rhs);

private:
    CDpllContext dpll_context_;
};

} // namespace tinysat

#endif // TINYSAT_CDPLLSOLVER_HPP_
