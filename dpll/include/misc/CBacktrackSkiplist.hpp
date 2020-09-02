#ifndef TINYSAT_CBACKTRACKSKIPLIST_HPP_
#define TINYSAT_CBACKTRACKSKIPLIST_HPP_

/**
 * @file CBacktrackSkiplist.hpp
 * @author geome_try
 * @date 2020
 */

#include <cstdint>

#include <vector>
#include <random>

/// @brief
namespace tinysat {

//----------------------------------------
// CBacktrackSkiplist<TData> class
//----------------------------------------

/// Skiplist able to extract/restore its nodes
/**
 * Holds vector of nodes with keys in ascending order
 */
template<typename TData>
class CBacktrackSkiplist
{
public:
    /// Null index representation
    static constexpr size_t NULL_IDX = static_cast<size_t>(-1);
    /// Width of the skiplist
    static constexpr size_t MAX_LOG = 64u/sizeof(size_t);

    class CNode; ///< Node holding extracted value
    class CIterator; ///< Iterator class.
    class CConstIterator; ///< ConstIterator class.

    using data_type = TData; ///< Type of internal data
    using node_type = CNode; ///< Type of node holding extracted value
    using iterator = CIterator; ///< BidirIt alias
    using const_iterator = CConstIterator; ///< ConstBidirIt alias

    // TODO: alignas(64u) for better caching
    
    /// Represents block of links
    /**
     * Holds MAX_LOG links as indices and implements array interface
     */
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

    /// Default ctor
    CBacktrackSkiplist() = default;

    /// Copy ctor from data vector
    explicit CBacktrackSkiplist(const std::vector<TData>&);
    /// Move ctor from data vector
    explicit CBacktrackSkiplist(std::vector<TData>&&);

    /// Ctor from range
    template<typename TIter>
    CBacktrackSkiplist(TIter&&, TIter&&);

    /// Rule of 5
    CBacktrackSkiplist             (const CBacktrackSkiplist&) = default;
    /// Rule of 5
    CBacktrackSkiplist& operator = (const CBacktrackSkiplist&) = default;

    /// Rule of 5
    CBacktrackSkiplist             (CBacktrackSkiplist&&) = default;
    /// Rule of 5
    CBacktrackSkiplist& operator = (CBacktrackSkiplist&&) = default;

    /// Returns size
    /**
     * @return elements' count
     */
    [[nodiscard]] size_t size() const noexcept
    {
        return size_;
    }

    /// Returns true iff empty
    /**
     * @return size() == 0
     */
    [[nodiscard]] bool empty() const noexcept
    {
        return size() == 0u;
    }

    /// STL-like interface 
    /**
     * @return iterator pointing to the front element
     * @see CIterator cbegin() end() cend()
     */
    [[nodiscard]] CIterator begin()
    {
        return CIterator(this, head_blk_[0u]);
    }

    /// STL-like interface 
    /**
     * @return const iterator pointing to the front element
     * @see CConstIterator cbegin() end() cend()
     */
    [[nodiscard]] CConstIterator begin() const
    {
        return CConstIterator(this, head_blk_[0u]);
    }

    /// STL-like interface 
    /**
     * @return const iterator pointing to the front element
     * @see CConstIterator begin() end() cend()
     */
    [[nodiscard]] CConstIterator cbegin() const
    {
        return begin();
    }

    /// STL-like interface 
    /**
     * @return const iterator pointing to the past-to-end element
     * @see CIterator begin() cbegin() cend()
     */
    [[nodiscard]] CIterator end()
    {
        return CIterator(this, NULL_IDX);
    }

    /// STL-like interface 
    /**
     * @return const iterator pointing to the past-to-end element
     * @see CConstIterator begin() cbegin() cend()
     */
    [[nodiscard]] CConstIterator end() const
    {
        return CConstIterator(this, NULL_IDX);
    }

    /// STL-like interface 
    /**
     * @return const iterator pointing to the past-to-end element
     * @see CConstIterator begin() cbegin() end()
     */
    [[nodiscard]] CConstIterator cend() const
    {
        return end();
    }

    /// Reassign all links basing on current data
    void reset();

    /// Extract corresonding value and links
    CNode extract(CIterator);
    /// Restore corresonding value and links
    CIterator restore(CNode&&);

    /// Find value
    [[nodiscard]] CIterator find(const TData&);
    /// Find value
    [[nodiscard]] CConstIterator find(const TData&) const;

    /// Simple check for errors
    [[nodiscard]] bool ok() const noexcept;

    /// Simple iostream-like output
    template<typename TStream>
    bool dump(TStream& stream) const noexcept;

protected:
    /// Gives an access to the corresponding value
    /**
     * @param [in] idx internal element index
     * @return corresponding value
     */
    [[nodiscard]] TData& at(const size_t idx)
    {
        return data_vec_[idx];
    }

    /// Gives an access to the corresponding value
    /**
     * @param [in] idx internal element's index
     * @return corresponding value
     */
    [[nodiscard]] const TData& at(const size_t idx) const
    {
        return data_vec_[idx];
    }

    /// Returns index of the prev element
    /**
     * @param [in] idx internal element's index
     * @return prev element's index
     */
    [[nodiscard]] size_t prev(const size_t idx) const
    {
        return (idx == NULL_IDX ? tail_blk_[0u] : prev_blk_vec_[idx][0u]);
    }

    /// Returns index of the next element
    /**
     * @param [in] idx internal element's index
     * @return next element's index
     */
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

/**
 * Holds index of extracted element
 */
template<typename TData>
class CBacktrackSkiplist<TData>::CNode
{
public:
    friend class CBacktrackSkiplist<TData>;

    /// Ctor from pointer to parent list and index
    CNode(CBacktrackSkiplist<TData>*, const size_t);

    CNode             (const CNode&) = delete; ///< Rule of 5
    CNode& operator = (const CNode&) = delete; ///< Rule of 5

    CNode             (CNode&&) = default; ///< Rule of 5
    CNode& operator = (CNode&&) = default; ///< Rule of 5

    /// Access corresponding element
    [[nodiscard]] TData& get() const;

private:
    CBacktrackSkiplist<TData>* list_ptr_;
    size_t elem_idx_;
};

//----------------------------------------
// CBacktrackSkiplist<TData>::CIterator class
//----------------------------------------

/**
 * Implements bidirectional iterator 
 * for the CBacktrackSkiplist template
 */
template<typename TData>
class CBacktrackSkiplist<TData>::CIterator
{
public:
    friend class CBacktrackSkiplist<TData>;

    using difference_type = std::ptrdiff_t; ///< Traits.
    using value_type = TData; ///< Traits.
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::bidirectional_iterator_tag; ///< Traits.

    /// Default ctor
    CIterator() = default;

    /// Ctor from list pointer and element index
    CIterator(CBacktrackSkiplist<TData>*, const size_t);

    CIterator             (const CIterator&) = default; ///< Rule of 5
    CIterator& operator = (const CIterator&) = default; ///< Rule of 5

    CIterator             (CIterator&&) = default; ///< Rule of 5
    CIterator& operator = (CIterator&&) = default; ///< Rule of 5

    [[nodiscard]] TData& operator * () const; ///< InputIt interface
    [[nodiscard]] TData* operator -> () const; ///< InputIt interface

    CIterator& operator ++ (); ///< InputIt interface
    CIterator& operator -- (); ///< BidirIt interface

    const CIterator operator ++ (int); ///< InputIt interface
    const CIterator operator -- (int); ///< BidirIt interface

    /// Implicit conversion to the CConstIterator
    operator CBacktrackSkiplist<TData>::CConstIterator () const;

    /// InputIt interface
    [[nodiscard]] friend 
    bool operator == (const CIterator& lhs, const CIterator& rhs)
    {
        return (lhs.list_ptr_ == rhs.list_ptr_) && 
               (lhs.elem_idx_ == rhs.elem_idx_);
    }

    /// InputIt interface
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

/**
 * Implements constant bidirectional iterator 
 * for the CBacktrackSkiplist template
 */
template<typename TData>
class CBacktrackSkiplist<TData>::CConstIterator
{
public:
    friend class CBacktrackSkiplist<TData>;

    using difference_type = std::ptrdiff_t; ///< Traits.
    using value_type = const TData; ///< Traits.
    using iterator_category = std::bidirectional_iterator_tag; ///< Traits.

    /// Default ctor
    CConstIterator() = default;

    /// Ctor from list pointer and element index
    CConstIterator(const CBacktrackSkiplist<TData>*, const size_t);

    CConstIterator             (const CConstIterator&) = default; ///< Rule of 5
    CConstIterator& operator = (const CConstIterator&) = default; ///< Rule of 5

    CConstIterator             (CConstIterator&&) = default; ///< Rule of 5
    CConstIterator& operator = (CConstIterator&&) = default; ///< Rule of 5

    [[nodiscard]] const TData& operator * () const; ///< InputIt interface
    [[nodiscard]] const TData* operator -> () const; ///< InputIt interface

    CConstIterator& operator ++ (); ///< InputIt interface
    CConstIterator& operator -- (); ///< BidirIt interface

    const CConstIterator operator ++ (int); ///< InputIt interface
    const CConstIterator operator -- (int); ///< BidirIt interface

    /// InputIt interface
    [[nodiscard]] friend 
    bool operator == (const CConstIterator& lhs, const CConstIterator& rhs)
    {
        return (lhs.list_ptr_ == rhs.list_ptr_) && 
               (lhs.elem_idx_ == rhs.elem_idx_);
    }

    /// InputIt interface
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

/**
 * @param [in] list_ptr pointer to the parent list
 * @param [in] elem_idx index of the extracted element
 */
template<typename TData>
CBacktrackSkiplist<TData>::CNode::
CNode(CBacktrackSkiplist<TData>* list_ptr, const size_t elem_idx):
    list_ptr_{ list_ptr },
    elem_idx_{ elem_idx }
{}

/**
 * @return reference to the extracted value
 */
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

/**
 * @param [in] list_ptr pointer to the parent list
 * @param [in] elem_idx index of the pointed-to element
 */
template<typename TData>
CBacktrackSkiplist<TData>::CIterator::
CIterator(CBacktrackSkiplist<TData>* list_ptr, 
          const size_t elem_idx):
    list_ptr_{ list_ptr },
    elem_idx_{ elem_idx }
{}

/**
 * @return reference to pointed-to value
 */
template<typename TData>
TData& 
CBacktrackSkiplist<TData>::CIterator::
operator * () const
{
    return list_ptr_->at(elem_idx_);
}

/**
 * @return pointer to pointed-to value
 */
template<typename TData>
TData* 
CBacktrackSkiplist<TData>::CIterator::
operator -> () const
{
    return &(*(*this)); // C++ is amazing (#1)
}

/**
 * @return *this
 */
template<typename TData>
typename CBacktrackSkiplist<TData>::CIterator&
CBacktrackSkiplist<TData>::CIterator::
operator ++ ()
{
    elem_idx_ = list_ptr_->next(elem_idx_);
    return *this;
}

/**
 * @return *this
 */
template<typename TData>
typename CBacktrackSkiplist<TData>::CIterator&
CBacktrackSkiplist<TData>::CIterator::
operator -- ()
{
    elem_idx_ = list_ptr_->prev(elem_idx_);
    return *this;
}

/**
 * @return old *this
 */
template<typename TData>
const typename CBacktrackSkiplist<TData>::CIterator
CBacktrackSkiplist<TData>::CIterator::
operator ++ (int)
{
    const auto that = *this;
    ++(*this);

    return that;
}

/**
 * @return old *this
 */
template<typename TData>
const typename CBacktrackSkiplist<TData>::CIterator
CBacktrackSkiplist<TData>::CIterator::
operator -- (int)
{
    const auto that = *this;
    --(*this);

    return that;
}

/**
 * @return corresponding CConstIterator
 */
template<typename TData>
CBacktrackSkiplist<TData>::CIterator::
operator CBacktrackSkiplist<TData>::CConstIterator () const
{
    return CConstIterator(list_ptr_, elem_idx_);
}

//----------------------------------------
// CBacktrackSkiplist<TData>::CIterator methods
//----------------------------------------

/**
 * @param [in] list_ptr pointer to the parent list
 * @param [in] elem_idx index of the pointed-to element
 */
template<typename TData>
CBacktrackSkiplist<TData>::CConstIterator::
CConstIterator(const CBacktrackSkiplist<TData>* list_ptr, 
               const size_t elem_idx):
    list_ptr_{ list_ptr },
    elem_idx_{ elem_idx }
{}

/**
 * @return reference to pointed-to value
 */
template<typename TData>
const TData& 
CBacktrackSkiplist<TData>::CConstIterator::
operator * () const
{
    return list_ptr_->at(elem_idx_);
}

/**
 * @return pointer to pointed-to value
 */
template<typename TData>
const TData* 
CBacktrackSkiplist<TData>::CConstIterator::
operator -> () const
{
    return &(*(*this)); // C++ is amazing (#2 because of copy-paste!)
}

/**
 * @return *this
 */
template<typename TData>
typename CBacktrackSkiplist<TData>::CConstIterator&
CBacktrackSkiplist<TData>::CConstIterator::
operator ++ ()
{
    elem_idx_ = list_ptr_->next(elem_idx_);
    return *this;
}

/**
 * @return *this
 */
template<typename TData>
typename CBacktrackSkiplist<TData>::CConstIterator&
CBacktrackSkiplist<TData>::CConstIterator::
operator -- ()
{
    elem_idx_ = list_ptr_->prev(elem_idx_);
    return *this;
}

/**
 * @return old *this
 */
template<typename TData>
const typename CBacktrackSkiplist<TData>::CConstIterator
CBacktrackSkiplist<TData>::CConstIterator::
operator ++ (int)
{
    const auto that = *this;
    ++(*this);

    return that;
}

/**
 * @return old *this
 */
template<typename TData>
const typename CBacktrackSkiplist<TData>::CConstIterator
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

/**
 * @param [in] data_vec data vector to copy
 */
template<typename TData>
CBacktrackSkiplist<TData>::
CBacktrackSkiplist(const std::vector<TData>& data_vec):
    data_vec_(data_vec)
{
    reset();
}

/**
 * @param [in,out] data_vec data vector to move
 */
template<typename TData>
CBacktrackSkiplist<TData>::
CBacktrackSkiplist(std::vector<TData>&& data_vec):
    data_vec_(std::move(data_vec))
{
    reset();
}

/**
 * @param [in] begin_it range begin iterator
 * @param [in] end_it range end iterator
 */
template<typename TData>
template<typename TIter>
CBacktrackSkiplist<TData>::
CBacktrackSkiplist(TIter&& begin_it, TIter&& end_it):
    data_vec_(std::forward<TIter>(begin_it), std::forward<TIter>(end_it))
{
    reset();
}

/**
 */
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

/**
 * @return true iff no errors are found
 */
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

/**
 * @return ok()
 * @see ok()
 */
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

/**
 * @param [in,out] node node representing previously extracted element
 * @return iterator pointing to the restored element
 */
template<typename TData>
typename CBacktrackSkiplist<TData>::CIterator
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

/**
 * @param [in] iter iterator pointing to the element to extract
 * @return node representing the extracted element
 */
template<typename TData>
typename CBacktrackSkiplist<TData>::CNode
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

/**
 * @param [in] value value to search for
 * @return iterator pointing to the found value or end() if no such exists
 */
template<typename TData>
typename CBacktrackSkiplist<TData>::CIterator
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

    if (idx == NULL_IDX || data_vec_[idx] < value || value < data_vec_[idx])
        idx = NULL_IDX;

    return CIterator(this, idx);
}

/**
 * @param [in] value value to search for
 * @return iterator pointing to the found value or end() if no such exists
 */
template<typename TData>
typename CBacktrackSkiplist<TData>::CConstIterator
CBacktrackSkiplist<TData>::find(const TData& value) const
{
    return const_cast<CBacktrackSkiplist*>(this)->find(value);
}

} // namespace tinysat

#endif // TINYSAT_CBACKTRACKSKIPLIST_HPP_
