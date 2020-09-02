#include "CDpllSortHeap.hpp"

/**
 * @file CDpllSortHeap.cpp
 * @author geome_try
 * @date 2020
 * @see CDpllSortHeap.hpp
 */

/// @brief
namespace tinysat {

/**
 * @param [in] formula formula to build heap from
 */
CDpllSortHeap::CDpllSortHeap(const SFormula& formula):
    lit_cnt_{ 2u*formula.params_cnt },
    size_{ lit_cnt_ },
    prior_sum_{ 0.0 },
    prior_vec_(lit_cnt_, 0.0),
    heap_vec_(1u + lit_cnt_, 0),
    heap_map_(lit_cnt_, 0u)
{
    int max_lit = static_cast<int>(formula.params_cnt);
    for (int lit = 1; lit <= max_lit; ++lit)
    {
        heap_vec_[lit2idx(lit) + 1u] = lit;
        heap_map_[lit2idx(lit)] = lit2idx(lit) + 1u;

        heap_vec_[lit2idx(-lit) + 1u] = -lit;
        heap_map_[lit2idx(-lit)] = lit2idx(-lit) + 1u;
    }

    for (const auto& clause : formula.clause_vec)
    {
        prior_sum_ += static_cast<double>(clause.size());
        for (int lit : clause)
            prior_vec_[lit2idx(lit)] += 1.0;
    }

    if (prior_sum_ < 1.0/BALANCE_SUM || BALANCE_SUM < prior_sum_)
        balance();

    heapify();
}

/**
 * @return heap's top literal
 */
int CDpllSortHeap::get() const
{
    return heap_vec_[1u];
}

/**
 * @param [in] lit literal to extract
 * @return extracted literal
 * @see restore()
 */
int CDpllSortHeap::extract(const int lit)
{
    [[unlikely]] if (prior_vec_[lit2idx(lit)] < 0.0)
        return 0;

    size_t it = heap_map_[lit2idx(lit)];
    prior_vec_[lit2idx(lit)] = abs(prior_vec_[lit2idx(lit)]) * -1.0;

    size_t old_it = it;
    while ((it = sift_dn(old_it)) != old_it)
        old_it = it;

    --size_;

    return lit;
}

/**
 * @param [in] lit extracted literal to restore
 * @return restored literal
 * @see extract()
 */
int CDpllSortHeap::restore(const int lit)
{
    [[unlikely]] if (prior_vec_[lit2idx(lit)] > 0.0)
        return 0;

    size_t it = heap_map_[lit2idx(lit)];
    prior_vec_[lit2idx(lit)] = abs(prior_vec_[lit2idx(lit)]);

    size_t old_it = it;
    while ((it = sift_up(old_it)) != old_it)
        old_it = it;

    ++size_;

    return lit;
}

/**
 * @param [in] lit literal to query
 * @return literal's priority
 * @see dec_prior()
 */
double CDpllSortHeap::get_prior(int lit) const
{
    return prior_vec_[lit2idx(lit)];
}

/**
 * @param [in] lit literal to decrease its priority
 * @see get_prior()
 */
void CDpllSortHeap::dec_prior(int lit)
{
    prior_sum_ -= std::abs(prior_vec_[lit2idx(lit)]);
    prior_vec_[lit2idx(lit)] *= DEC_FACTOR;
    prior_sum_ += std::abs(prior_vec_[lit2idx(lit)]);

    size_t it = heap_map_[lit2idx(lit)];
    size_t old_it = it;
    while ((it = sift_up(old_it)) != old_it)
        old_it = it;

    if (prior_sum_ < 1.0/BALANCE_SUM || BALANCE_SUM < prior_sum_)
        balance();
}

/**
 * @param [in] it element's internal index
 * @see sift_dn()
 */
size_t CDpllSortHeap::sift_up(size_t it)
{
    if (it < 2u)
        return it;

    auto& cur_lit = heap_vec_[it];
    auto& up_lit = heap_vec_[it/2u];

    if (prior_vec_[lit2idx(up_lit)] < prior_vec_[lit2idx(cur_lit)])
    {
        std::swap(cur_lit, up_lit);
        std::swap(heap_map_[lit2idx(cur_lit)], heap_map_[lit2idx(up_lit)]);

        it /= 2u;
    }

    return it;
}

/**
 * @param [in] it element's internal index
 * @see sift_up()
 */
size_t CDpllSortHeap::sift_dn(size_t it)
{
    if (heap_vec_.size() < 2u*(it + 1u))
        return it;

    auto& cur_lit = heap_vec_[it];
    auto& lt_lit = heap_vec_[2u*it + 0u];
    auto& rt_lit = heap_vec_[2u*it + 1u];

    if (prior_vec_[lit2idx(cur_lit)] < 
        std::max(prior_vec_[lit2idx(lt_lit)], prior_vec_[lit2idx(rt_lit)]))
    {
        if (prior_vec_[lit2idx(lt_lit)] < prior_vec_[lit2idx(rt_lit)])
        {
            std::swap(cur_lit, rt_lit);
            std::swap(heap_map_[lit2idx(cur_lit)], heap_map_[lit2idx(rt_lit)]);

            it = 2u*it + 1u;
        }
        else
        {
            std::swap(cur_lit, lt_lit);
            std::swap(heap_map_[lit2idx(cur_lit)], heap_map_[lit2idx(lt_lit)]);

            it = 2u*it + 0u;
        }
    }

    return it;
}

/**
 * @see balance()
 */
void CDpllSortHeap::heapify()
{
    for (size_t it = lit_cnt_; it != 0u; --it)
        sift_up(it);
}

/**
 * @see heapify()
 */
void CDpllSortHeap::balance()
{
    double factor = BALANCE_SUM/prior_sum_;

    prior_sum_ = 0.0;
    for (size_t idx = 0u; idx < lit_cnt_; ++idx)
        prior_sum_ += (prior_vec_[lit_cnt_ - 1u - idx] *= factor);
}

/**
 * @return true if no errors detected
 * @see dump()
 */
bool CDpllSortHeap::ok() const noexcept
{
    bool result = true;
    result = result && 
        (1.0 / BALANCE_SUM < prior_sum_) &&
        (prior_sum_ < BALANCE_SUM) &&
        (lit_cnt_ == heap_vec_.size() + 1u) &&
        (lit_cnt_ == heap_map_.size()) &&
        (lit_cnt_ == prior_vec_.size());

    return result;
}

/**
 * @param [in,out] stream iostream-like stream object
 * @return ok()
 * @see ok()
 */
template<typename TStream>
bool CDpllSortHeap::dump(TStream& stream) const noexcept
{
    size_t cur_max = 1u;
    size_t beg_idx = 1u;
    while (beg_idx <= lit_cnt_)
    {
        for (size_t cur_idx = 0u; 
             cur_idx < cur_max && beg_idx + cur_idx <= lit_cnt_; 
             ++cur_idx)
        {
            int lit = heap_vec_[beg_idx + cur_idx];
            stream << 
                "(" << lit << 
                "," << prior_vec_[lit2idx(lit)] << 
                ") ";
        }

        stream << "\n";

        beg_idx += cur_max;
        cur_max *= 2u;
    }

    return ok();
}

} // namespace tinysat
