#ifndef TINYSAT_CGENERALSOLVER_HPP_
#define TINYSAT_CGENERALSOLVER_HPP_

#include <memory>

#include "CMatchIterator.hpp"
#include "CException.hpp"
#include "SFormula.hpp"

namespace tinysat {

// SAT solver implementing a DPLL algorithm
//
// TODO:
// - To improve the solving algorithm
// - To imlement LegasySolver concept
//
class CGeneralSolver
{
public:
    class CContext;

    using context_t = CContext;
    using CIterator = CMatchIterator<const CGeneralSolver>;

    explicit CGeneralSolver(const SFormula&);
    explicit CGeneralSolver(SFormula&&);

    CGeneralSolver             (CGeneralSolver&&) = default;
    CGeneralSolver& operator = (CGeneralSolver&&) = default;

    void reset(const SFormula&);
    void reset(SFormula&&);

    [[nodiscard]] std::unique_ptr<CContext> context() const;
    void proceed(std::unique_ptr<CContext>&) const;

    [[nodiscard]] CIterator begin() const;
    [[nodiscard]] CIterator end() const;

protected:
    void init();
    bool propagate(CContext&) const;

private:
    SFormula formula_;
};

// Context incapsulating data for the CGeneralSolver algorithm
//
// TODO:
// - To imlement LegasyContext concept
//
class CGeneralSolver::CContext
{
public:
    friend class CGeneralSolver;

    explicit CContext(const SMatch&);
    explicit CContext(SMatch&&);

    CContext             (const CContext&) = default;
    CContext& operator = (const CContext&) = default;

    CContext             (CContext&&) = default;
    CContext& operator = (CContext&&) = default;

    [[nodiscard]] SMatch match() const;

    friend bool operator == (const CContext& lhs, const CContext& rhs);
    friend bool operator != (const CContext& lhs, const CContext& rhs);

private:
    SMatch match_;
};

} // namespace tinysat

#endif // TINYSAT_CGENERALSOLVER_HPP_
