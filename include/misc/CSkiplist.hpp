#ifndef TINYSAT_CSKIPLIST_HPP_
#define TINYSAT_CSKIPLIST_HPP_

#include <cstdint>

#include <vector>
#include <random>

namespace tinysat {

template<typename TData>
class CSkiplist
{
public:
    static constexpr std::size_t MAX_LOG = 24u;
    static constexpr std::size_t NULL_IT = static_cast<std::size_t>(-1);

    class CIterator
    {
    public:
        friend class CSkiplist;

        CIterator();
        CIterator(CSkiplist*, std::size_t);

        CIterator& operator ++ ();
        CIterator operator ++ (int);
        CIterator& operator -- ();
        CIterator operator -- (int);

        TData& operator * () const;
        TData* operator -> () const;

        bool operator == (const CIterator&) const;
        bool operator != (const CIterator&) const;

    private:
        CSkiplist* list_ptr_;
        std::size_t elem_idx_;
    };

    class CConstIterator
    {
    public:
        friend class CSkiplist;

        CConstIterator();
        CConstIterator(const CSkiplist*, std::size_t);

        CConstIterator& operator ++ ();
        CConstIterator operator ++ (int);
        CConstIterator& operator -- ();
        CConstIterator operator -- (int);

        const TData& operator * () const;
        const TData* operator -> () const;

        bool operator == (const CConstIterator&) const;
        bool operator != (const CConstIterator&) const;

    private:
        const CSkiplist* list_ptr_;
        std::size_t elem_idx_;
    };

    CSkiplist();
    explicit CSkiplist(std::size_t);

    CSkiplist             (const CSkiplist&) = default;
    CSkiplist& operator = (const CSkiplist&) = default;

    CSkiplist             (CSkiplist&&) = default;
    CSkiplist& operator = (CSkiplist&&) = default;

    ~CSkiplist() = default;

    CIterator begin();
    CConstIterator begin() const;
    CConstIterator cbegin() const;

    CIterator end();
    CConstIterator end() const;
    CConstIterator cend() const;

    bool ok() const noexcept;

    template<typename TStream>
    bool dump(TStream& stream) const noexcept;

    CIterator      find(const TData&);
    CConstIterator find(const TData&) const;

    CIterator      insert(const TData&);

    CIterator      erase(CIterator);
    CConstIterator erase(CConstIterator);

    CIterator      upper_bound(const TData&);
    CConstIterator upper_bound(const TData&) const;

    CIterator      lower_bound(const TData&);
    CConstIterator lower_bound(const TData&) const;

    void reserve(std::size_t);

protected:
    std::size_t find_it(const TData&) const;
    std::size_t insert_it(const TData&);
    std::size_t erase_it(std::size_t);
    std::size_t upper_bound_it(const TData&) const;
    std::size_t lower_bound_it(const TData&) const;

    std::size_t search(const TData&) const;

    std::size_t next(std::size_t it) const;
    std::size_t prev(std::size_t it) const;

          TData& data(std::size_t);
    const TData& data(std::size_t) const;

private:
    std::random_device randomizer_;

    std::size_t size_;
    std::size_t capacity_;

    std::size_t free_;
    std::size_t head_arr_[MAX_LOG];
    std::size_t tail_arr_[MAX_LOG];

    std::vector<TData> data_vec_;
    std::vector<std::size_t> size_vec_;
    std::vector<std::size_t> prev_vec_arr_[MAX_LOG]; 
    std::vector<std::size_t> next_vec_arr_[MAX_LOG]; 
};

template<typename TData>
CSkiplist<TData>::CIterator::
CIterator():
    list_ptr_(nullptr),
    elem_idx_(NULL_IT)
{}

template<typename TData>
CSkiplist<TData>::CIterator::
CIterator(CSkiplist* list_ptr_set, std::size_t elem_idx_set):
    list_ptr_(list_ptr_set),
    elem_idx_(elem_idx_set)
{}

template<typename TData>
typename
CSkiplist<TData>::CIterator& 
CSkiplist<TData>::CIterator::
operator ++ ()
{
    elem_idx_ = list_ptr_->next(elem_idx_);

    return (*this);
}

template<typename TData>
typename
CSkiplist<TData>::CIterator 
CSkiplist<TData>::CIterator::
operator ++ (int)
{
    CIterator result = (*this);
    ++(*this);

    return result;
}

template<typename TData>
typename
CSkiplist<TData>::CIterator& 
CSkiplist<TData>::CIterator::
operator -- ()
{
    elem_idx_ = list_ptr_->prev(elem_idx_);

    return (*this);
}

template<typename TData>
typename
CSkiplist<TData>::CIterator 
CSkiplist<TData>::CIterator::
operator -- (int)
{
    CIterator result = (*this);
    --(*this);

    return result;
}

template<typename TData>
TData& 
CSkiplist<TData>::CIterator::
operator * () const
{
    return list_ptr_->data(elem_idx_);
}

template<typename TData>
TData* 
CSkiplist<TData>::CIterator::
operator -> () const
{
    return &list_ptr_->data(elem_idx_);
}

template<typename TData>
bool 
CSkiplist<TData>::CIterator::
operator == (const typename CSkiplist<TData>::CIterator& other) const
{
    return (list_ptr_ == other.list_ptr_) && 
           (elem_idx_ == other.elem_idx_);
}

template<typename TData>
bool 
CSkiplist<TData>::CIterator::
operator != (const typename CSkiplist<TData>::CIterator& other) const
{
    return !(*this == other);
}

template<typename TData>
CSkiplist<TData>::CConstIterator::
CConstIterator():
    list_ptr_(nullptr),
    elem_idx_(NULL_IT)
{}

template<typename TData>
CSkiplist<TData>::CConstIterator::
CConstIterator(const CSkiplist* list_ptr_set, std::size_t elem_idx_set):
    list_ptr_(list_ptr_set),
    elem_idx_(elem_idx_set)
{}

template<typename TData>
typename
CSkiplist<TData>::CConstIterator::CConstIterator& 
CSkiplist<TData>::CConstIterator::
operator ++ ()
{
    elem_idx_ = list_ptr_->next(elem_idx_);

    return (*this);
}

template<typename TData>
typename
CSkiplist<TData>::CConstIterator::CConstIterator 
CSkiplist<TData>::CConstIterator::
operator ++ (int)
{
    CConstIterator result = (*this);
    ++(*this);

    return result;
}

template<typename TData>
typename
CSkiplist<TData>::CConstIterator::CConstIterator& 
CSkiplist<TData>::CConstIterator::
operator -- ()
{
    elem_idx_ = list_ptr_->prev(elem_idx_);

    return (*this);
}

template<typename TData>
typename
CSkiplist<TData>::CConstIterator::CConstIterator 
CSkiplist<TData>::CConstIterator::
operator -- (int)
{
    CConstIterator result = (*this);
    --(*this);

    return result;
}

template<typename TData>
const TData& 
CSkiplist<TData>::CConstIterator::
operator * () const
{
    return list_ptr_->data(elem_idx_);
}

template<typename TData>
const TData* 
CSkiplist<TData>::CConstIterator::
operator -> () const
{
    return &list_ptr_->data(elem_idx_);
}

template<typename TData>
bool 
CSkiplist<TData>::CConstIterator::
operator == (const typename CSkiplist<TData>::CConstIterator& other) const
{
    return (list_ptr_ == other.list_ptr_) && 
           (elem_idx_ == other.elem_idx_);
}

template<typename TData>
bool 
CSkiplist<TData>::CConstIterator::
operator != (const typename CSkiplist<TData>::CConstIterator& other) const
{
    return !(*this == other);
}

template<typename TData>
CSkiplist<TData>::CSkiplist():
    randomizer_(),
    size_(0u),
    capacity_(0u),
    free_(NULL_IT),
    head_arr_{},
    tail_arr_{},
    data_vec_(),
    size_vec_(),
    prev_vec_arr_{},
    next_vec_arr_{}
{
    for (std::size_t log = 0u; log < MAX_LOG; ++log)
    {
        next_vec_arr_[log].assign(size_, NULL_IT);
        prev_vec_arr_[log].assign(size_, NULL_IT);

        head_arr_[log] = tail_arr_[log] = NULL_IT;
    }
}

template<typename TData>
CSkiplist<TData>::CSkiplist(std::size_t size_set):
    randomizer_(),
    size_(size_set),
    capacity_(size_),
    free_(NULL_IT),
    head_arr_(),
    tail_arr_(),
    data_vec_(size_),
    size_vec_(size_set),
    prev_vec_arr_{},
    next_vec_arr_{}
{
    for (std::size_t log = 0u; log < MAX_LOG; ++log)
    {
        next_vec_arr_[log].assign(size_, NULL_IT);
        prev_vec_arr_[log].assign(size_, NULL_IT);

        head_arr_[log] = tail_arr_[log] = NULL_IT;
    }

    for (std::size_t i = 0u; i < size_; ++i)
    {
        next_vec_arr_[0u][i] = i + 1u;
        prev_vec_arr_[0u][i] = i - 1u;
    }

    if (size_ != 0u)
    {
        head_arr_[0u] = 0u;
        tail_arr_[0u] = size_ - 1u;

        next_vec_arr_[0u].back() = NULL_IT;
        prev_vec_arr_[0u].front() = NULL_IT;
    }

    for (auto& elem : size_vec_)
        elem = 1u;

    for (std::size_t log = 1u; log < MAX_LOG; ++log)
    {
        for (std::size_t it = head_arr_[log - 1u]; it != NULL_IT; 
             it = next_vec_arr_[log - 1u][it])
        {
            if (randomizer_()%2u)
            {
                if (head_arr_[log] == NULL_IT)
                {
                    next_vec_arr_[log][it] = prev_vec_arr_[log][it] = NULL_IT;
                    head_arr_[log] = tail_arr_[log] = it;
                }
                else
                {
                    next_vec_arr_[log][tail_arr_[log]] = it;
                    prev_vec_arr_[log][it] = tail_arr_[log]; 
                    next_vec_arr_[log][it] = NULL_IT;
                    tail_arr_[log] = it;
                }

                ++size_vec_[it];
            }
        }
    }
}

template<typename TData>
typename
CSkiplist<TData>::CIterator 
CSkiplist<TData>::begin()
{
    return CIterator(this, head_arr_[0u]);
}

template<typename TData>
typename
CSkiplist<TData>::CConstIterator 
CSkiplist<TData>::begin() const
{
    return CConstIterator(this, head_arr_[0u]);
}

template<typename TData>
typename
CSkiplist<TData>::CConstIterator 
CSkiplist<TData>::cbegin() const
{
    return CConstIterator(this, head_arr_[0u]);
}

template<typename TData>
typename
CSkiplist<TData>::CIterator 
CSkiplist<TData>::end()
{
    return CIterator(this, NULL_IT);
}

template<typename TData>
typename
CSkiplist<TData>::CConstIterator 
CSkiplist<TData>::end() const
{
    return CConstIterator(this, NULL_IT);
}

template<typename TData>
typename
CSkiplist<TData>::CConstIterator 
CSkiplist<TData>::cend() const
{
    return CConstIterator(this, NULL_IT);
}

template<typename TData>
typename
CSkiplist<TData>::CIterator 
CSkiplist<TData>::find(const TData& value)
{
    return CIterator(this, find_it(value));
}

template<typename TData>
typename
CSkiplist<TData>::CConstIterator 
CSkiplist<TData>::find(const TData& value) const
{
    return CConstIterator(this, find_it(value));
}

template<typename TData>
typename
CSkiplist<TData>::CIterator 
CSkiplist<TData>::insert(const TData& value)
{
    return CIterator(this, insert_it(value));
}

template<typename TData>
typename
CSkiplist<TData>::CIterator 
CSkiplist<TData>::erase(CIterator iter)
{
    return CIterator(this, erase_it(iter.elem_idx_));
}

template<typename TData>
typename
CSkiplist<TData>::CConstIterator 
CSkiplist<TData>::erase(CConstIterator iter)
{
    return CConstIterator(this, erase_it(iter.elem_idx_));
}

template<typename TData>
typename
CSkiplist<TData>::CIterator 
CSkiplist<TData>::upper_bound(const TData& value)
{
    return CIterator(this, upper_bound_it(value));
}

template<typename TData>
typename
CSkiplist<TData>::CConstIterator 
CSkiplist<TData>::upper_bound(const TData& value) const
{
    return CConstIterator(this, upper_bound_it(value));
}

template<typename TData>
typename
CSkiplist<TData>::CIterator 
CSkiplist<TData>::lower_bound(const TData& value)
{
    return CIterator(this, lower_bound_it(value));
}

template<typename TData>
typename
CSkiplist<TData>::CConstIterator 
CSkiplist<TData>::lower_bound(const TData& value) const
{
    return CConstIterator(this, lower_bound_it(value));
}

template<typename TData>
bool CSkiplist<TData>::ok() const noexcept
{
    bool result = true;

    result = result && (size_ < capacity_ || 
                        (free_ != NULL_IT && size_ == capacity_));

    result = result && (data_vec_.size() == capacity_);
    result = result && (size_vec_.size() == capacity_);

    for (std::size_t log = 0u; log < MAX_LOG; ++log)
    {
        result = result && (prev_vec_arr_[log].size() == capacity_);
        result = result && (next_vec_arr_[log].size() == capacity_);

        result = result && ((head_arr_[log] == NULL_IT && 
                             tail_arr_[log] == NULL_IT) ||
                            (head_arr_[log] != NULL_IT && 
                             tail_arr_[log] != NULL_IT));
    }

    return result;
}

template<typename TData>
template<typename TStream>
bool CSkiplist<TData>::dump(TStream& stream) const noexcept
{
    bool result = ok();

    stream << "size: " << size_ << "\n";
    stream << "capacity: " << capacity_ << "\n";
    if (free_ != NULL_IT)
        stream << "free: " << free_ << "\n";
    else
        stream << "free: X\n";

    stream << "size_vec: \n";
    for (std::size_t i = 0u; i < capacity_; ++i)
        stream << "[" << i << ", " << size_vec_[i] << "] ";

    stream << "\n";
    stream << "data_vec: \n";
    for (std::size_t i = 0u; i < capacity_; ++i)
        stream << "[" << i << ", " /*<< data_vec_[i] <<*/ "] ";

    stream << "\n";
    stream << "next_vec_arr, prew_vec_arr:\n";
    for (std::size_t log = 0u; log < MAX_LOG; ++log)
    {
        if (head_arr_[log] == NULL_IT && tail_arr_[log] == NULL_IT)
            break;

        for (std::size_t i = 0u; i < capacity_; ++i)
        {
            if (next_vec_arr_[log][i] != NULL_IT)
                stream << "[" << i << ", " << next_vec_arr_[log][i] << "] ";
            else
                stream << "[" << i << ", X] ";
        }
        stream << "[head " << head_arr_[log] << "]\n";

        for (std::size_t i = 0u; i < capacity_; ++i)
        {
            if (prev_vec_arr_[log][i] != NULL_IT)
                stream << "[" << i << ", " << prev_vec_arr_[log][i] << "] ";
            else
                stream << "[" << i << ", X] ";
        }
        stream << "[tail " << tail_arr_[log] << "]\n";
    }

    return result;
}

template<typename TData>
void 
CSkiplist<TData>::reserve(std::size_t capacity_set)
{
    if (capacity_set <= capacity_)
        return;

    data_vec_.resize(capacity_set);
    size_vec_.resize(capacity_set, 0u);

    for (std::size_t log = 0u; log < MAX_LOG; ++log)
    {
        next_vec_arr_[log].resize(capacity_set, NULL_IT);
        prev_vec_arr_[log].resize(capacity_set, NULL_IT);
    }

    std::size_t cur_free = free_;
    if (cur_free != NULL_IT)
    {
        while (next_vec_arr_[0u][cur_free] != NULL_IT)
            cur_free = next_vec_arr_[0u][cur_free]; 

        next_vec_arr_[0u][cur_free] = capacity_;
    }
    else
    {
        free_ = capacity_;
    }

    while (capacity_ < capacity_set)
    {
        next_vec_arr_[0u][capacity_] = capacity_ + 1u;

        ++capacity_;
    }

    next_vec_arr_[0u][capacity_ - 1u] = NULL_IT;
}

template<typename TData>
std::size_t 
CSkiplist<TData>::find_it(const TData& value) const
{
    std::size_t result = search(value);

    if (result == NULL_IT || 
        (data_vec_[result] < value) || 
        (value < data_vec_[result]))
    {
        result = NULL_IT;
    }

    return result;
}

template<typename TData>
std::size_t 
CSkiplist<TData>::insert_it(const TData& value)
{
    std::size_t result = 0u;
    std::size_t it = search(value);

    if (free_ == NULL_IT)
    {
        free_ = data_vec_.size();
        data_vec_.emplace_back();
        size_vec_.push_back(0u);

        for (std::size_t log = 0u; log < MAX_LOG; ++log)
        {
            next_vec_arr_[log].push_back(NULL_IT);
            prev_vec_arr_[log].push_back(NULL_IT);
        }

        ++capacity_;
    }

    result = free_;
    free_ = next_vec_arr_[0u][free_];
    
    data_vec_[result] = value;
    size_vec_[result] = 1u;
    for (std::size_t log = 1u; log < MAX_LOG; ++log)
    {
        if (randomizer_()%2u)
            ++size_vec_[result];
        else
            break;
    }

    std::size_t prev_it = NULL_IT;
    std::size_t next_it = NULL_IT;
    if (it != NULL_IT)
    {
        next_it = next_vec_arr_[0u][it];
        prev_it = it;
    }
    else
    {
        next_it = head_arr_[0u];
        prev_it = NULL_IT;
    }

    for (std::size_t log = 0u; log < size_vec_[result]; ++log)
    {
        while (next_it != NULL_IT && size_vec_[next_it] <= log)
            next_it = next_vec_arr_[log - 1u][next_it];

        while (prev_it != NULL_IT && size_vec_[prev_it] <= log)
            prev_it = prev_vec_arr_[log - 1u][prev_it];

        next_vec_arr_[log][result] = next_it;
        prev_vec_arr_[log][result] = prev_it;

        if (prev_it != NULL_IT) next_vec_arr_[log][prev_it] = result;
        else                    head_arr_[log] = result;

        if (next_it != NULL_IT) prev_vec_arr_[log][next_it] = result;
        else                    tail_arr_[log] = result;
    }

    ++size_;

    return result;
}

template<typename TData>
std::size_t 
CSkiplist<TData>::erase_it(std::size_t it)
{
    std::size_t result = next_vec_arr_[0u][it];

    for (std::size_t log = 0u; log < size_vec_[it]; ++log)
    {
        std::size_t prev_it = prev_vec_arr_[log][it];
        std::size_t next_it = next_vec_arr_[log][it];

        if (prev_it != NULL_IT) next_vec_arr_[log][prev_it] = next_it;
        else                    head_arr_[log] = next_it;

        if (next_it != NULL_IT) prev_vec_arr_[log][next_it] = prev_it;
        else                    tail_arr_[log] = prev_it;
    }

    size_vec_[it] = 0u;
    prev_vec_arr_[0u][it] = NULL_IT;
    next_vec_arr_[0u][it] = free_;
    free_ = it;

    --size_;

    return result;
}

template<typename TData>
std::size_t 
CSkiplist<TData>::upper_bound_it(const TData& value) const
{
    std::size_t result = search(value);

    result = (result == NULL_IT ? 
              head_arr_[0u] : 
              next_vec_arr_[0u][result]);

    return result;
}

template<typename TData>
std::size_t 
CSkiplist<TData>::lower_bound_it(const TData& value) const
{
    std::size_t result = search(value);

    return result;
}

template<typename TData>
std::size_t 
CSkiplist<TData>::search(const TData& value) const
{
    std::size_t result = NULL_IT;

    std::size_t log = MAX_LOG - 1u;
    //comparison via overflow
    while (log < MAX_LOG && (head_arr_[log] == NULL_IT || 
           value < data_vec_[head_arr_[log]]))
    {
        --log;
    }

    std::size_t it = (log < MAX_LOG ? head_arr_[log] : NULL_IT);
    //comparison via overflow
    while (log < MAX_LOG)
    {
        while (it != NULL_IT && data_vec_[it] < value)
        {
            it = next_vec_arr_[log][it];
        }

        if (it == NULL_IT)
            it = tail_arr_[log];
        else if (value < data_vec_[it])
            it = prev_vec_arr_[log][it];
        else
            break;

        --log;
    }

    result = it;

    return result;
}

template<typename TData>
std::size_t CSkiplist<TData>::next(std::size_t it) const
{
    return next_vec_arr_[0u][it];
}

template<typename TData>
std::size_t CSkiplist<TData>::prev(std::size_t it) const
{
    return (it == NULL_IT ? tail_arr_[it] : prev_vec_arr_[0u][it]);
}

template<typename TData>
TData& CSkiplist<TData>::data(std::size_t it)
{
    return data_vec_[it];
}

template<typename TData>
const TData& CSkiplist<TData>::data(std::size_t it) const
{
    return data_vec_[it];
}

} // namespace tinysat

#endif // TINYSAT_CSKIPLIST_HPP_
