#ifndef TINYSAT_DPLL_CDPLLSORTHEAP_HPP_
#define TINYSAT_DPLL_CDPLLSORTHEAP_HPP_

#include "SFormula.hpp"

namespace tinysat {

class CDpllSortHeap
{
public:
    static constexpr double BALANCE_SUM = 16.0;
    static constexpr double DEC_FACTOR = 0.95;
    using node_type = int;

    CDpllSortHeap() = default;
    explicit CDpllSortHeap(const SFormula&);

    CDpllSortHeap             (const CDpllSortHeap&) = default;
    CDpllSortHeap& operator = (const CDpllSortHeap&) = default;
    CDpllSortHeap             (CDpllSortHeap&&) = default;
    CDpllSortHeap& operator = (CDpllSortHeap&&) = default;

    [[nodiscard]] size_t size() const noexcept
    {
        return size_;
    }

    [[nodiscard]] size_t empty() const noexcept
    {
        return (size() == 0u);
    }

    int get() const;
    int extract(const int);
    int restore(const int);

    double get_prior(const int) const;
    void dec_prior(const int);

    [[nodiscard]] bool ok() const noexcept;

    template<typename TStream>
    bool dump(TStream&) const noexcept;

protected:
    size_t sift_up(size_t);
    size_t sift_dn(size_t);
    void heapify();
    void balance();

    [[nodiscard]] constexpr inline 
    size_t lit2idx(const int lit) const noexcept
    {
        return (lit < 0 ? 
                static_cast<size_t>(lit_cnt_/2u + (-lit - 1)) : 
                static_cast<size_t>(lit - 1));
    }

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
