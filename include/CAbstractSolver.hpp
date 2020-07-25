#ifndef TINYSAT_CABSTRACTSOLVER_HPP_
#define TINYSAT_CABSTRACTSOLVER_HPP_

#include <memory>

#include "SFormula.hpp"

namespace tinysat {

class CAbstractSolver
{
public:
    class CContext;

    CAbstractSolver() = default;

    CAbstractSolver             (CAbstractSolver&&) = default;
    CAbstractSolver& operator = (CAbstractSolver&&) = default;

    virtual ~CAbstractSolver() = default;

    [[nodiscard]]
    virtual std::unique_ptr<CContext> 
    solve(const SFormula&) = 0;
};

class CAbstractSolver::CContext
{
public:
    CContext() = default;

    CContext             (CContext&&) = default;
    CContext& operator = (CContext&&) = default;

    virtual ~CContext() = default;

    [[nodiscard]]
    virtual SMatch match() const = 0;

    [[nodiscard]]
    virtual bool proceed() = 0;
};

} // namespace tinysat

#endif // TINYSAT_CABSTRACTSOLVER_HPP_
