#ifndef TINYSAT_CBACKTRACKSKIPLIST_HPP_
#define TINYSAT_CBACKTRACKSKIPLIST_HPP_

#include <cstdint>

#include <vector>
#include <random>

namespace tinysat {

//----------------------------------------
// CBacktrackSkiplist<TData> class
//----------------------------------------
template<typename TData>
class CBacktrackSkiplist
{
public:
    static constexpr size_t NULL_IDX = static_cast<size_t>(-1);
    static constexpr size_t MAX_LOG = 64u/sizeof(size_t);

    class CNode;
    class CIterator;
    class CConstIterator;

    using data_type = TData;
    using node_type = CNode;
    using iterator = CIterator;
    using const_iterator = CConstIterator;

    // TODO: alignas(64u) for better caching
    struct SIndexBlock
    {
    public:
        SIndexBlock() noexcept
        {
            for (size_t log = 0u; log < MAX_LOG; ++log)
                idx_arr[log] = NULL_IDX;
        }

        [[nodiscard]] 
        inline size_t& operator [] (const size_t idx) noexcept 
        { return idx_arr[idx]; }

        [[nodiscard]] 
        inline const size_t& operator [] (const size_t idx) const noexcept 
        { return idx_arr[idx]; }

    private:
        size_t idx_arr[MAX_LOG];
    };

    CBacktrackSkiplist() = default;

    explicit CBacktrackSkiplist(const std::vector<TData>&);
    explicit CBacktrackSkiplist(std::vector<TData>&&);

    template<typename TIter>
    CBacktrackSkiplist(TIter&&, TIter&&);

    CBacktrackSkiplist             (const CBacktrackSkiplist&) = default;
    CBacktrackSkiplist& operator = (const CBacktrackSkiplist&) = default;

    CBacktrackSkiplist             (CBacktrackSkiplist&&) = default;
    CBacktrackSkiplist& operator = (CBacktrackSkiplist&&) = default;

    [[nodiscard]] size_t size() const noexcept
    {
        return size_;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return size() == 0u;
    }

    [[nodiscard]] CIterator begin()
    {
        return CIterator(this, head_blk_[0u]);
    }

    [[nodiscard]] CConstIterator begin() const
    {
        return CConstIterator(this, head_blk_[0u]);
    }

    [[nodiscard]] CConstIterator cbegin() const
    {
        return begin();
    }

    [[nodiscard]] CIterator end()
    {
        return CIterator(this, NULL_IDX);
    }

    [[nodiscard]] CConstIterator end() const
    {
        return CConstIterator(this, NULL_IDX);
    }

    [[nodiscard]] CConstIterator cend() const
    {
        return end();
    }

    void reset();

    CNode extract(CIterator);
    CIterator restore(CNode&&);

    [[nodiscard]] CIterator find(const TData&);
    [[nodiscard]] CConstIterator find(const TData&) const;

    [[nodiscard]] bool ok() const noexcept;

    template<typename TStream>
    bool dump(TStream& stream) const noexcept;

protected:
    [[nodiscard]] TData& at(const size_t idx)
    {
        return data_vec_[idx];
    }

    [[nodiscard]] const TData& at(const size_t idx) const
    {
        return data_vec_[idx];
    }

    [[nodiscard]] size_t prev(const size_t idx) const
    {
        return (idx == NULL_IDX ? tail_blk_[0u] : prev_blk_vec_[idx][0u]);
    }

    [[nodiscard]] size_t next(const size_t idx) const
    {
        return next_blk_vec_[idx][0u];
    }

private:
    size_t size_ = 0u;

    SIndexBlock head_blk_;
    SIndexBlock tail_blk_;

    std::vector<SIndexBlock> prev_blk_vec_;
    std::vector<SIndexBlock> next_blk_vec_;

    std::vector<TData> data_vec_;
};

//----------------------------------------
// CBacktrackSkiplist<TData>::CNode class
//----------------------------------------
template<typename TData>
class CBacktrackSkiplist<TData>::CNode
{
public:
    friend class CBacktrackSkiplist<TData>;

    CNode(CBacktrackSkiplist<TData>*, const size_t);

    CNode             (const CNode&) = delete;
    CNode& operator = (const CNode&) = delete;

    CNode             (CNode&&) = default;
    CNode& operator = (CNode&&) = default;

    [[nodiscard]] TData& get() const;

private:
    CBacktrackSkiplist<TData>* list_ptr_;
    size_t elem_idx_;
};

//----------------------------------------
// CBacktrackSkiplist<TData>::CIterator class
//----------------------------------------
template<typename TData>
class CBacktrackSkiplist<TData>::CIterator
{
public:
    friend class CBacktrackSkiplist<TData>;

    using difference_type = std::ptrdiff_t;
    using value_type = TData;
    using iterator_category = std::bidirectional_iterator_tag;

    CIterator() = default;
    CIterator(CBacktrackSkiplist<TData>*, const size_t);

    CIterator             (const CIterator&) = default;
    CIterator& operator = (const CIterator&) = default;

    CIterator             (CIterator&&) = default;
    CIterator& operator = (CIterator&&) = default;

    [[nodiscard]] TData& operator * () const;
    [[nodiscard]] TData* operator -> () const;

    CIterator& operator ++ ();
    CIterator& operator -- ();

    const CIterator operator ++ (int);
    const CIterator operator -- (int);

    operator CBacktrackSkiplist<TData>::CConstIterator () const;

    [[nodiscard]] friend 
    bool operator == (const CIterator& lhs, const CIterator& rhs)
    {
        return (lhs.list_ptr_ == rhs.list_ptr_) && 
               (lhs.elem_idx_ == rhs.elem_idx_);
    }

    [[nodiscard]] friend 
    bool operator != (const CIterator& lhs, const CIterator& rhs)
    {
        return !(lhs == rhs);
    }

private:
    CBacktrackSkiplist<TData>* list_ptr_ = nullptr;
    size_t elem_idx_ = CBacktrackSkiplist<TData>::NULL_IDX;
};

//----------------------------------------
// CBacktrackSkiplist<TData>::CConstIterator class
//----------------------------------------
template<typename TData>
class CBacktrackSkiplist<TData>::CConstIterator
{
public:
    friend class CBacktrackSkiplist<TData>;

    using difference_type = std::ptrdiff_t;
    using value_type = const TData;
    using iterator_category = std::bidirectional_iterator_tag;

    CConstIterator() = default;
    CConstIterator(const CBacktrackSkiplist<TData>*, const size_t);

    CConstIterator             (const CConstIterator&) = default;
    CConstIterator& operator = (const CConstIterator&) = default;

    CConstIterator             (CConstIterator&&) = default;
    CConstIterator& operator = (CConstIterator&&) = default;

    [[nodiscard]] const TData& operator * () const;
    [[nodiscard]] const TData* operator -> () const;

    CConstIterator& operator ++ ();
    CConstIterator& operator -- ();

    const CConstIterator operator ++ (int);
    const CConstIterator operator -- (int);

    [[nodiscard]] friend 
    bool operator == (const CConstIterator& lhs, const CConstIterator& rhs)
    {
        return (lhs.list_ptr_ == rhs.list_ptr_) && 
               (lhs.elem_idx_ == rhs.elem_idx_);
    }

    [[nodiscard]] friend 
    bool operator != (const CConstIterator& lhs, const CConstIterator& rhs)
    {
        return !(lhs == rhs);
    }

private:
    const CBacktrackSkiplist<TData>* list_ptr_ = nullptr;
    size_t elem_idx_ = CBacktrackSkiplist<TData>::NULL_IDX;
};

//----------------------------------------
// CBacktrackSkiplist<TData>::CNode methods
//----------------------------------------
template<typename TData>
CBacktrackSkiplist<TData>::CNode::
CNode(CBacktrackSkiplist<TData>* list_ptr, const size_t elem_idx):
    list_ptr_{ list_ptr },
    elem_idx_{ elem_idx }
{}

template<typename TData>
TData&
CBacktrackSkiplist<TData>::CNode::
get() const
{
    return list_ptr_->at(elem_idx_);
}

//----------------------------------------
// CBacktrackSkiplist<TData>::CIterator methods
//----------------------------------------
template<typename TData>
CBacktrackSkiplist<TData>::CIterator::
CIterator(CBacktrackSkiplist<TData>* list_ptr, 
          const size_t elem_idx):
    list_ptr_{ list_ptr },
    elem_idx_{ elem_idx }
{}

template<typename TData>
TData& 
CBacktrackSkiplist<TData>::CIterator::
operator * () const
{
    return list_ptr_->at(elem_idx_);
}

template<typename TData>
TData* 
CBacktrackSkiplist<TData>::CIterator::
operator -> () const
{
    return &(*(*this)); // C++ is amazing (#1)
}

template<typename TData>
CBacktrackSkiplist<TData>::CIterator& 
CBacktrackSkiplist<TData>::CIterator::
operator ++ ()
{
    elem_idx_ = list_ptr_->next(elem_idx_);
    return *this;
}

template<typename TData>
CBacktrackSkiplist<TData>::CIterator& 
CBacktrackSkiplist<TData>::CIterator::
operator -- ()
{
    elem_idx_ = list_ptr_->prev(elem_idx_);
    return *this;
}

template<typename TData>
const CBacktrackSkiplist<TData>::CIterator 
CBacktrackSkiplist<TData>::CIterator::
operator ++ (int)
{
    const auto that = *this;
    ++(*this);

    return that;
}

template<typename TData>
const CBacktrackSkiplist<TData>::CIterator 
CBacktrackSkiplist<TData>::CIterator::
operator -- (int)
{
    const auto that = *this;
    --(*this);

    return that;
}

template<typename TData>
CBacktrackSkiplist<TData>::CIterator::
operator CBacktrackSkiplist<TData>::CConstIterator () const
{
    return CConstIterator(list_ptr_, elem_idx_);
}

//----------------------------------------
// CBacktrackSkiplist<TData>::CIterator methods
//----------------------------------------
template<typename TData>
CBacktrackSkiplist<TData>::CConstIterator::
CConstIterator(const CBacktrackSkiplist<TData>* list_ptr, 
               const size_t elem_idx):
    list_ptr_{ list_ptr },
    elem_idx_{ elem_idx }
{}

template<typename TData>
const TData& 
CBacktrackSkiplist<TData>::CConstIterator::
operator * () const
{
    return list_ptr_->at(elem_idx_);
}

template<typename TData>
const TData* 
CBacktrackSkiplist<TData>::CConstIterator::
operator -> () const
{
    return &(*(*this)); // C++ is amazing (#2 because of copy-paste!)
}

template<typename TData>
CBacktrackSkiplist<TData>::CConstIterator& 
CBacktrackSkiplist<TData>::CConstIterator::
operator ++ ()
{
    elem_idx_ = list_ptr_->next(elem_idx_);
    return *this;
}

template<typename TData>
CBacktrackSkiplist<TData>::CConstIterator& 
CBacktrackSkiplist<TData>::CConstIterator::
operator -- ()
{
    elem_idx_ = list_ptr_->prev(elem_idx_);
    return *this;
}

template<typename TData>
const CBacktrackSkiplist<TData>::CConstIterator 
CBacktrackSkiplist<TData>::CConstIterator::
operator ++ (int)
{
    const auto that = *this;
    ++(*this);

    return that;
}

template<typename TData>
const CBacktrackSkiplist<TData>::CConstIterator 
CBacktrackSkiplist<TData>::CConstIterator::
operator -- (int)
{
    const auto that = *this;
    --(*this);

    return that;
}

//----------------------------------------
// CBacktrackSkiplist<TData> methods
//----------------------------------------
template<typename TData>
CBacktrackSkiplist<TData>::
CBacktrackSkiplist(const std::vector<TData>& data_vec):
    data_vec_(data_vec)
{
    reset();
}

template<typename TData>
CBacktrackSkiplist<TData>::
CBacktrackSkiplist(std::vector<TData>&& data_vec):
    data_vec_(std::move(data_vec))
{
    reset();
}

template<typename TData>
template<typename TIter>
CBacktrackSkiplist<TData>::
CBacktrackSkiplist(TIter&& begin_it, TIter&& end_it):
    data_vec_(std::forward<TIter>(begin_it), std::forward<TIter>(end_it))
{
    reset();
}

template<typename TData>
void CBacktrackSkiplist<TData>::reset()
{
    // static can be ommited due to the standard
    static thread_local std::random_device STATIC_randomizer = {};

    size_ = data_vec_.size();
    head_blk_ = tail_blk_ = {};
    prev_blk_vec_.assign(size_, {});
    next_blk_vec_.assign(size_, {});

    std::sort(std::begin(data_vec_), std::end(data_vec_));

    for (size_t idx = 0u; idx < size_; ++idx)
    {
        next_blk_vec_[idx][0u] = idx + 1u;
        prev_blk_vec_[idx][0u] = idx - 1u;
    }

    if (size_ != 0u)
    {
        head_blk_[0u] = 0u;
        tail_blk_[0u] = size_ - 1u;

        next_blk_vec_.back()[0u] = prev_blk_vec_.front()[0u] = NULL_IDX;
    }

    for (size_t log = 1u; log < MAX_LOG; ++log)
    {
        for (size_t idx = head_blk_[log - 1u]; idx != NULL_IDX; 
             idx = next_blk_vec_[idx][log - 1u])
        {
            if (STATIC_randomizer()%2u)
            {
                if (head_blk_[log] == NULL_IDX)
                {
                    next_blk_vec_[idx][log] = NULL_IDX;
                    prev_blk_vec_[idx][log] = NULL_IDX;
                    head_blk_[log] = tail_blk_[log] = idx;
                }
                else
                {
                    next_blk_vec_[tail_blk_[log]][log] = idx;
                    prev_blk_vec_[idx][log] = tail_blk_[log]; 
                    next_blk_vec_[idx][log] = NULL_IDX;
                    tail_blk_[log] = idx;
                }
            }
        }
    }
}

template<typename TData>
bool CBacktrackSkiplist<TData>::ok() const noexcept
{
    bool result = true;

    result = result && (size_ <= data_vec_.size());
    result = result && (data_vec_.size() == prev_blk_vec_.size());
    result = result && (data_vec_.size() == next_blk_vec_.size());

    for (size_t log = 0u; log < MAX_LOG; ++log)
        result = result && 
            ((head_blk_[log] == NULL_IDX && tail_blk_[log] == NULL_IDX) ||
             (head_blk_[log] != NULL_IDX && tail_blk_[log] != NULL_IDX));

    return result;
}

template<typename TData>
template<typename TStream>
bool CBacktrackSkiplist<TData>::dump(TStream& stream) const noexcept
{
    bool result = ok();

    stream << "size: " << size_ << "\n";

    stream << "data_vec: \n";
    for (size_t i = 0u; i < data_vec_.size(); ++i)
        stream << "[" << i << ", " /*<< data_vec_[i] <<*/ "] ";

    stream << "\n";
    stream << "next_vec_arr, prew_vec_arr:\n";
    for (size_t log = 0u; log < MAX_LOG; ++log)
    {
        if (head_blk_[log] == NULL_IDX && tail_blk_[log] == NULL_IDX)
            break;

        for (size_t i = 0u; i < data_vec_.size(); ++i)
        {
            if (next_blk_vec_[i][log] != NULL_IDX)
                stream << "[" << i << ", " << next_blk_vec_[i][log] << "] ";
            else
                stream << "[" << i << ", X] ";
        }
        stream << "[head " << head_blk_[log] << "]\n";

        for (size_t i = 0u; i < data_vec_.size(); ++i)
        {
            if (prev_blk_vec_[i][log] != NULL_IDX)
                stream << "[" << i << ", " << prev_blk_vec_[i][log] << "] ";
            else
                stream << "[" << i << ", X] ";
        }
        stream << "[tail " << tail_blk_[log] << "]\n";
    }

    return result;
}

template<typename TData>
CBacktrackSkiplist<TData>::CIterator 
CBacktrackSkiplist<TData>::restore(CNode&& node)
{
    const size_t idx = std::move(node).elem_idx_;

    for (size_t log = 0u; log < MAX_LOG; ++log)
    {
        size_t prev_idx = prev_blk_vec_[idx][log];
        size_t next_idx = prev_blk_vec_[idx][log];

        if (prev_idx == NULL_IDX && next_idx == NULL_IDX)
            break;

        if (prev_idx == NULL_IDX) head_blk_[log] = idx;
        else                      next_blk_vec_[prev_idx][log] = idx;

        if (next_idx == NULL_IDX) tail_blk_[log] = idx;
        else                      prev_blk_vec_[next_idx][log] = idx;
    }

    ++size_;

    return CIterator(this, idx);
}

template<typename TData>
CBacktrackSkiplist<TData>::CNode 
CBacktrackSkiplist<TData>::extract(const CIterator iter)
{
    const size_t idx = iter.elem_idx_;

    for (size_t log = 0u; log < MAX_LOG; ++log)
    {
        size_t prev_idx = prev_blk_vec_[idx][log];
        size_t next_idx = next_blk_vec_[idx][log];

        if (prev_idx == NULL_IDX && next_idx == NULL_IDX)
            break;

        if (prev_idx != NULL_IDX) next_blk_vec_[prev_idx][log] = next_idx;
        else                      head_blk_[log] = next_idx;

        if (next_idx != NULL_IDX) prev_blk_vec_[next_idx][log] = prev_idx;
        else                      tail_blk_[log] = prev_idx;
    }

    --size_;

    return CNode(this, idx);
}

template<typename TData>
CBacktrackSkiplist<TData>::CIterator 
CBacktrackSkiplist<TData>::find(const TData& value)
{
    size_t log = MAX_LOG - 1u;
    //comparison via unsigned overflow
    while (log < MAX_LOG && (head_blk_[log] == NULL_IDX || 
           value < data_vec_[head_blk_[log]]))
    {
        --log;
    }

    size_t idx = (log < MAX_LOG ? head_blk_[log] : NULL_IDX);
    //comparison via unsigned overflow
    while (log < MAX_LOG)
    {
        while (idx != NULL_IDX && data_vec_[idx] < value)
            idx = next_blk_vec_[idx][log];

        if (idx == NULL_IDX)             idx = tail_blk_[log];
        else if (value < data_vec_[idx]) idx = prev_blk_vec_[idx][log];
        else                             break;

        --log;
    }

    return CIterator(this, idx);
}

template<typename TData>
CBacktrackSkiplist<TData>::CConstIterator 
CBacktrackSkiplist<TData>::find(const TData& value) const
{
    return const_cast<CBacktrackSkiplist*>(this)->find(value);
}

} // namespace tinysat

#endif // TINYSAT_CBACKTRACKSKIPLIST_HPP_
