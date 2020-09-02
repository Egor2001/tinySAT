#ifndef TINYSAT_CGENERALSOLVER_HPP_
#define TINYSAT_CGENERALSOLVER_HPP_

/**
 * @file
 * @author geome_try
 * @date 2020
 */

#include <memory>

#include "CMatchIterator.hpp"
#include "CException.hpp"
#include "SFormula.hpp"

/// @brief
namespace tinysat {

// SAT solver implementing a DPLL algorithm
//
// TODO:
// - To improve the solving algorithm
// - To imlement LegasySolver concept

/// Class representing solver for general-case SAT
class CGeneralSolver;

/**
 * holds formula and implements algorithm over the context
 */
class CGeneralSolver
{
public:
    /// Class representing the algorithm-specific data
    class CContext;

    using context_t = CContext; ///< context alias
    using CIterator = CMatchIterator<const CGeneralSolver>; ///< iterator alias

    explicit CGeneralSolver(const SFormula&); ///< Copy ctor from SAT formula
    explicit CGeneralSolver(SFormula&&); ///< Move ctor from SAT formula

    CGeneralSolver             (CGeneralSolver&&) = default; ///< Move ctor
    CGeneralSolver& operator = (CGeneralSolver&&) = default; ///< Move operator

    void reset(const SFormula&); ///< Reset internal formula via copy
    void reset(SFormula&&); ///< Reset internal formula via move

    /// Get context corresponding to the formula
    [[nodiscard]] std::unique_ptr<CContext> context() const;
    /// Update context to find next solution
    void proceed(std::unique_ptr<CContext>&) const;

    /// Get iterator to the first solution
    [[nodiscard]] CIterator begin() const;

    /// Get iterator past to the last solution
    [[nodiscard]] CIterator end() const;

protected:
    /// Initialize solver's internal data corresponding to the formula
    void init();

    /// Try to calculate next solution
    bool propagate(CContext&) const;

private:
    SFormula formula_;
};

// Context incapsulating data for the CGeneralSolver algorithm
//
// TODO:
// - To imlement LegasyContext concept
//

/**
 */
class CGeneralSolver::CContext
{
public:
    friend class CGeneralSolver;

    explicit CContext(const SMatch&); ///< Copy ctor from the match object
    explicit CContext(SMatch&&); ///< Move ctor from the match object

    CContext             (const CContext&) = default; ///< Rule of 5
    CContext& operator = (const CContext&) = default; ///< Rule of 5

    CContext             (CContext&&) = default; ///< Rule of 5
    CContext& operator = (CContext&&) = default; ///< Rule of 5

    /// Get corresponding match object
    [[nodiscard]] SMatch match() const;

    /// Comparison operator
    friend bool operator == (const CContext& lhs, const CContext& rhs);
    /// Comparison operator
    friend bool operator != (const CContext& lhs, const CContext& rhs);

private:
    SMatch match_;
};

} // namespace tinysat

#endif // TINYSAT_CGENERALSOLVER_HPP_
