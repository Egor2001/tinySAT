#ifndef TINYSAT_DPLL_CDPLLASSIGNMENT_HPP_
#define TINYSAT_DPLL_CDPLLASSIGNMENT_HPP_

/**
 * @file CDpllAssignment.hpp
 * @author geome_try
 * @date 2020
 */

#include <stack>

#include "CException.hpp"
#include "SFormula.hpp"
#include "SMatch.hpp"
#include "CDpllSortHeap.hpp"

/// @brief
namespace tinysat {

/// Represents SAT parameters' assignment for the DPLL algorithm
class CDpllAssignment;

/**
 * Holds the heap of literals with their priorities 
 * and the vector of parameters' values
 */
class CDpllAssignment
{
public:
    /// Represents state for backtracking
    struct SState
    {
        size_t log_idx; ///< size of the assigned paraneters' stack
    };

    /// Default Ctor
    CDpllAssignment() = default;

    /// Ctor from the SAT formula
    explicit CDpllAssignment(const SFormula&);

    /// Rule of 5
    CDpllAssignment             (const CDpllAssignment&) = default;
    /// Rule of 5
    CDpllAssignment& operator = (const CDpllAssignment&) = default;

    CDpllAssignment             (CDpllAssignment&&) = default; ///< Rule of 5
    CDpllAssignment& operator = (CDpllAssignment&&) = default; ///< Rule of 5

    /// Returns parameters' assignment
    /**
     * @return match for current parameters
     */
    [[nodiscard]] const SMatch& match() const noexcept
    {
        return match_;
    }

    /// Returns current state
    [[nodiscard]] SState get_state() const;

    /// Backtrack to the given state
    void backtrack(const SState&);

    int request() const; ///< Return next literal to assign true to
    void proceed(const int); ///< Assign true to the given literal

    /// Checks if all literals are assigned
    /**
     * @return true if all literals are assigned
     * @see request()
     */
    [[nodiscard]] bool full() const
    {
        return (request() == 0);
    }

    /// Provides simple error checking
    [[nodiscard]] bool ok() const noexcept;

    /// Provides simple stream output
    template<typename TStream>
    bool dump(TStream& stream) const noexcept;

protected:
    void assign(const int); ///< Assigns true to the given literal
    void revert(const int); ///< Reverts assignment of the given literal

private:
    SMatch match_;
    CDpllSortHeap sort_heap_;
    std::stack<int> log_stk_;
};

} // namespace tinysat

#endif // TINYSAT_DPLL_CDPLLASSIGNMENT_HPP_
