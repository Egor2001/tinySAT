#ifndef TINYSAT_DPLL_CDPLLSORTHEAP_HPP_
#define TINYSAT_DPLL_CDPLLSORTHEAP_HPP_

/**
 * @file CDpllSortHeap.hpp
 * @author geome_try
 * @date 2020
 */

#include "SFormula.hpp"

/// @brief
namespace tinysat {

/// Heap for literal priority management
class CDpllSortHeap
{
public:
    /// Equals B, where sum of all priorities must be inside (1/B, B)
    static constexpr double BALANCE_SUM = 16.0;

    /// Equals F, where literal's priority is F times decreased after conflict
    static constexpr double DEC_FACTOR = 0.95;

    /// Type that is returned from extract() and passed into restore()
    using node_type = int;

    CDpllSortHeap() = default; ///< Default Ctor

    /// Ctor from existing SAT formula
    explicit CDpllSortHeap(const SFormula&);

    CDpllSortHeap             (const CDpllSortHeap&) = default; ///< Rule of 5
    CDpllSortHeap& operator = (const CDpllSortHeap&) = default; ///< Rule of 5
    CDpllSortHeap             (CDpllSortHeap&&) = default; ///< Rule of 5
    CDpllSortHeap& operator = (CDpllSortHeap&&) = default; ///< Rule of 5

    /// Returns active literal's count
    /**
     * @return active literal's count
     */
    [[nodiscard]] size_t size() const noexcept
    {
        return size_;
    }

    /// Checks if active part of heap is empty
    /**
     * @return size() == 0
     */
    [[nodiscard]] bool empty() const noexcept
    {
        return (size() == 0u);
    }

    
    int get() const; ///< Returns min literal
    int extract(const int); ///< Extracts literal from heap
    int restore(const int); ///< Restores previously extracted literal

    double get_prior(const int) const; ///< Returns literal priority
    void dec_prior(const int); ///< Decreases conflict literal priority

    /// Provides simple error checking
    [[nodiscard]] bool ok() const noexcept;

    /// Provides simple stream output
    template<typename TStream>
    bool dump(TStream&) const noexcept;

protected:
    size_t sift_up(size_t); ///< See binary heap interface
    size_t sift_dn(size_t); ///< See binary heap interface
    void heapify(); ///< See binary heap interface

    /// Scales priorities with respect to BALANCE_SUM
    void balance();

    /// Converts literal to its internal index
    /**
     * @param[in] lit literal
     * @return index of lit
     * @see idx2lit()
     */
    [[nodiscard]] constexpr inline 
    size_t lit2idx(const int lit) const noexcept
    {
        return (lit < 0 ? 
                static_cast<size_t>(lit_cnt_/2u + (-lit - 1)) : 
                static_cast<size_t>(lit - 1));
    }

    /// Converts internal index to the corresponding literal
    /**
     * @param[in] idx index
     * @return literal with index idx
     * @see lit2idx()
     */
    [[nodiscard]] constexpr inline 
    int idx2lit(const size_t idx) const noexcept
    {
        return (idx < lit_cnt_/2u ? 
                +static_cast<int>(idx + 1u) : 
                -static_cast<int>(idx + 1u - lit_cnt_/2u));
    }

private:
    size_t lit_cnt_ = 0u;
    size_t size_ = 0u;
    double prior_sum_ = 0.0;
    std::vector<double> prior_vec_;
    std::vector<int> heap_vec_;
    std::vector<size_t> heap_map_;
};

} // namespace tinysat

#endif // TINYSAT_DPLL_CDPLLSORTHEAP_HPP_
