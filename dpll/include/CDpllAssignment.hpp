#ifndef TINYSAT_DPLL_CDPLLASSIGNMENT_HPP_
#define TINYSAT_DPLL_CDPLLASSIGNMENT_HPP_

#include <stack>

#include "CException.hpp"
#include "SFormula.hpp"
#include "SMatch.hpp"
#include "CDpllSortHeap.hpp"

namespace tinysat {

class CDpllAssignment
{
public:
    struct SState
    {
        size_t log_idx;
    };

    CDpllAssignment() = default;
    explicit CDpllAssignment(const SFormula&);

    CDpllAssignment             (const CDpllAssignment&) = default;
    CDpllAssignment& operator = (const CDpllAssignment&) = default;
    CDpllAssignment             (CDpllAssignment&&) = default;
    CDpllAssignment& operator = (CDpllAssignment&&) = default;

    [[nodiscard]] const SMatch& match() const noexcept
    {
        return match_;
    }

    [[nodiscard]] SState get_state() const;
    void backtrack(const SState&);

    int request() const;
    void proceed(const int);

    [[nodiscard]] bool full() const
    {
        return (request() == 0);
    }

    [[nodiscard]] bool ok() const noexcept;

    template<typename TStream>
    bool dump(TStream& stream) const noexcept;

protected:
    void assign(const int);
    void revert(const int);

private:
    SMatch match_;
    CDpllSortHeap sort_heap_;
    std::stack<int> log_stk_;
};

} // namespace tinysat

#endif // TINYSAT_DPLL_CDPLLASSIGNMENT_HPP_
