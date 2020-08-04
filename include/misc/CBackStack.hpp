#ifndef TINYSAT_MISC_CBACKSTACK_HPP_
#define TINYSAT_MISC_CBACKSTACK_HPP_

#include <vector>

namespace tinysat {

template<typename TData>
class CBackStack
{
public:
    class CIterator;

    using data_type = TData;
    using iterator = CIterator;

    CBackStack() = default;
    explicit CBackStack(size_t);

    CBackStack             (const CBackStack&) = default;
    CBackStack& operator = (const CBackStack&) = default;
    CBackStack             (CBackStack&&) = default;
    CBackStack& operator = (CBackStack&&) = default;

    TData& top()
    const TData& top() const
    void pop();

    void push(const TData&);
    void push(TData&&);

    template<typename FOnPop>
    void backtrack(CIterator, FOnPop);

private:
    size_t size_;
    std::vector<TData> data_vec_;
};

} // namespace tinysat

#endif // TINYSAT_MISC_CBACKSTACK_HPP_
