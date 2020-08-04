#include <bits/stdc++.h>

//low-latency heap for POD-types
template<typename TData, size_t NCapacity>
class CQuickHeap 
{
public:
    CQuickHeap()
    {
        data_arr_.resize(NCapacity);
        pidx_vec_.push_back(0u);
    }

    template<typename TIter>
    CQuickHeap(TIter begin, TIter end)
    {
        data_arr_.resize(NCapacity);
        for (TIter it = begin; it != end && size_ < NCapacity; ++it, ++size_)
            data_arr_[size_] = *it;

        pidx_vec_.push_back(size_);
        inc_qsort();
    }

    size_t size() const noexcept { return size_; }
    bool empty() const noexcept  { return (size_ == 0u); }

    const TData& top() const
    { 
        return data_arr_[idx_]; 
    }

    void pop()
    {
        if (pidx_vec_.back() == idx_)
            pidx_vec_.pop_back();

        idx_ = (idx_ + 1u)%NCapacity;
        --size_;

        inc_qsort();
    }

    void push(const TData& value)
    {
        size_t it = 0u;
        while (it < pidx_vec_.size())
        {
            size_t old_pidx = pidx_vec_[it];
            size_t new_pidx = (pidx_vec_[it] + 1u)%NCapacity;
            data_arr_[new_pidx] = data_arr_[old_pidx];
            pidx_vec_[it] = new_pidx;

            if (it + 1u == pidx_vec_.size() || 
                !(value < data_arr_[pidx_vec_[it + 1u]]))
            {
                data_arr_[old_pidx] = value;
                break;
            }
            else
            {
                size_t next_new_pidx = (pidx_vec_[it + 1u] + 1u)%NCapacity;
                data_arr_[old_pidx] = data_arr_[next_new_pidx];
            }

            ++it;
        }

        ++size_;
        inc_qsort();
    }

private:
    void inc_qsort()
    {
        while (pidx_vec_.back() != idx_)
        {
            size_t prefix = (pidx_vec_.back() + NCapacity - idx_)%NCapacity;
            size_t pidx = (idx_ + rand_dev_()%prefix)%NCapacity;
            pidx_vec_.push_back(partition(pidx));
        }
    }

    size_t partition(size_t pidx)
    {
        size_t left = idx_, right = pidx_vec_.back() - 1u;
        std::swap(data_arr_[pidx], data_arr_[right]);
        for (size_t i = left; i != right; i = (i + 1u)%NCapacity)
        {
            if (data_arr_[i] < data_arr_[right])
            {
                std::swap(data_arr_[i], data_arr_[left]);
                left = (left + 1u)%NCapacity;
            }
        }
        std::swap(data_arr_[right], data_arr_[left]);
         
        return left;
    }

private:
    std::random_device rand_dev_;

    size_t idx_ = 0u;
    size_t size_ = 0u;

    std::vector<size_t> pidx_vec_;
    std::vector<TData> data_arr_;
};

