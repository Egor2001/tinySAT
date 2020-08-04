#ifndef TINYSAT_MISC_CCHANGELOG_HPP_
#define TINYSAT_MISC_CCHANGELOG_HPP_

#include <stack>

namespace tinysat {

template<typename TData>
class CChangeLog
{
public:
    struct SEntry
    {
        size_t level;
        TData data;
    };

    [[nodiscard]] bool empty() const noexcept
    {
        return entry_stk_.empty();
    }

    void push(const TData& data, size_t level)
    {
        entry_stk_.push(SEntry{ .level = level, .data = data });
    }

    void push(TData&& data, size_t level)
    {
        entry_stk_.push(SEntry{ .level = level, .data = std::move(data) });
    }

    void pop()
    {
        entry_stk_.pop();
    }

    [[nodiscard]] size_t level() const
    {
        return (empty() ? 0 : entry_stk_.top().level);
    }

    [[nodiscard]] TData& top()
    {
        return entry_stk_.top().data;
    }

    [[nodiscard]] const TData& top() const
    {
        return entry_stk_.top().data;
    }

private:
    std::stack<SEntry> entry_stk_;
};

} // namespace tinysat

#endif // TINYSAT_MISC_CCHANGELOG_HPP_
