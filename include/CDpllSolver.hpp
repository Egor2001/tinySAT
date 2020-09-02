#ifndef TINYSAT_CDPLLSOLVER_HPP_
#define TINYSAT_CDPLLSOLVER_HPP_

/**
 * @file
 * @author geome_try
 * @date 2020
 */

#include <memory>

#include "CException.hpp"
#include "CMatchIterator.hpp"
#include "SFormula.hpp"
#include "SMatch.hpp"

#include "include/CDpllContext.hpp"

#include "spdlog/spdlog.h"

/// @brief
namespace tinysat {

/// Solver based on the DPLL algo
class CDpllSolver;

/**
 * Holds SAT formula and logger
 */
class CDpllSolver
{
public:
    /// Context incapsulating DPLL data
    class CContext;

    using context_t = CContext; ///< Context alias
    using CIterator = CMatchIterator<const CDpllSolver>; ///< Iterator alias

    /// Ctor from the general SAT formula and logger
    CDpllSolver(const SFormula&, std::shared_ptr<spdlog::logger>);

    /// Ctor from the rvalue SAT formula and logger
    CDpllSolver(SFormula&&, std::shared_ptr<spdlog::logger>);

    CDpllSolver             (const CDpllSolver&) = default; ///< Rule of 5
    CDpllSolver& operator = (const CDpllSolver&) = default; ///< Rule of 5
    CDpllSolver             (CDpllSolver&&) = default; ///< Rule of 5
    CDpllSolver& operator = (CDpllSolver&&) = default; ///< Rule of 5

    /// Get corresponding context
    [[nodiscard]] std::unique_ptr<context_t> context() const;
    /// Update context up to the next solution
    void proceed(std::unique_ptr<context_t>&) const;

    /// Get iterator pointing to the first solution
    [[nodiscard]] CIterator begin() const;
    /// Get iterator pointing past to the last solution
    [[nodiscard]] CIterator end() const;

private:
    SFormula formula_;
    std::shared_ptr<spdlog::logger> logger_;
};

/**
 * Holds DPLL data and logger
 */
struct CDpllSolver::CContext
{
public:
    friend class CDpllSolver;

    /// Ctor from the general SAT formula and logger
    CContext(const SFormula&, std::shared_ptr<spdlog::logger>);

    CContext             (const CContext&) = default; ///< Rule of 5
    CContext& operator = (const CContext&) = default; ///< Rule of 5
    CContext             (CContext&&) = default; ///< Rule of 5
    CContext& operator = (CContext&&) = default; ///< Rule of 5

    /// Get corresponding match object
    [[nodiscard]] SMatch match() const;

    /// Comparison operator
    [[nodiscard]] friend 
    bool operator == (const CContext& lhs, const CContext& rhs);
    /// Comparison operator
    [[nodiscard]] friend 
    bool operator != (const CContext& lhs, const CContext& rhs);

private:
    CDpllContext dpll_context_;
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace tinysat

#endif // TINYSAT_CDPLLSOLVER_HPP_
