#ifndef TINYSAT_CBACKTRACKLIST_HPP_
#define TINYSAT_CBACKTRACKLIST_HPP_

/**
 * @file CBacktrackList.hpp
 * @author geome_try
 * @date 2020
 */

#include <memory>
#include <iterator>

/// @brief
namespace tinysat {

//----------------------------------------
// CBacktrackList<TData> class
//----------------------------------------

/// List able to extract/restore its nodes
/**
 * Implements linked list with nodes separately allocated on the heap.
 * Holds head and tail entry and size.
 */
template<typename TData>
class CBacktrackList
{
public:
    class CNode; ///< Node holding extracted value
    class CIterator; ///< Bidirectional iterator
    class CConstIterator; ///< Constant bidirectional iterator

    using data_type = TData; ///< Type of elements
    using node_type = CNode; ///< Type of single node
    using iterator = CIterator; ///< Alias for iterator
    using const_iterator = CConstIterator; ///< Alias for constant iterator

    /// Entry holding value and 2 links
    /**
     * Every entry owns all its successors.
     */
    struct SEntry
    {
        TData data; ///< Value
        std::unique_ptr<SEntry> next; ///< Successor owning link
        SEntry* prev; ///< Predecessor link
    };

    /// Default ctor
    CBacktrackList() = default;

    /// Ctor from range
    template<typename TIter>
    CBacktrackList(TIter&&, TIter&&);

    /// Deep copy ctor
    CBacktrackList             (const CBacktrackList&) = default;
    /// Deep copy assignment
    CBacktrackList& operator = (const CBacktrackList&) = default;

    /// Move ctor
    CBacktrackList             (CBacktrackList&&) = default;
    /// Move assignment
    CBacktrackList& operator = (CBacktrackList&&) = default;

    /// Returns current elements' count
    /**
     * @return size of list
     * @see empty()
     */
    [[nodiscard]] size_t size() const noexcept
    { 
        return size_; 
    }

    /// Returns true iff list is empty
    /**
     * @return true iff list is empty
     * @see size()
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
        return CIterator(this, head_.get());
    }

    /// STL-like interface
    /**
     * @return const_iterator pointing to the front element
     * @see CConstIterator cbegin() end() cend()
     */
    [[nodiscard]] CConstIterator begin() const
    {
        return CConstIterator(this, head_.get());
    }

    /// STL-like interface
    /**
     * @return const_iterator pointing to the front element
     * @see CConstIterator begin() end() cend()
     */
    [[nodiscard]] CConstIterator cbegin() const
    {
        return begin();
    }

    /// STL-like interface
    /**
     * @return iterator pointing to the past-to-end element
     * @see CIterator begin() cbegin() end() cend()
     */
    [[nodiscard]] CIterator end()
    {
        return CIterator(this, nullptr);
    }

    /// STL-like interface
    /**
     * @return const_iterator pointing to the past-to-end element
     * @see CConstIterator begin() cbegin() cend()
     */
    [[nodiscard]] CConstIterator end() const
    {
        return CConstIterator(this, nullptr);
    }

    /// STL-like interface
    /**
     * @return const_iterator pointing to the past-to-end element
     * @see CConstIterator begin() cbegin() end()
     */
    [[nodiscard]] CConstIterator cend() const
    {
        return end();
    }

    /// Extracts corresponding entry
    CNode extract(const CIterator);
    /// Restore corresponding entry
    CIterator restore(CNode&&);

    /// Pushes back new entry holding copied given value
    CIterator push_back(const TData&);
    /// Pushes back new entry holding moved given value
    CIterator push_back(TData&&);

private:
    size_t size_ = 0u;
    std::unique_ptr<SEntry> head_;
    SEntry* tail_ = nullptr;
};

//----------------------------------------
// CBacktrackList<TData>::CNode class
//----------------------------------------

/// Extracted entry's owner class
/**
 * Holds owning pointer to extracted SEntry
 * @see SEntry
 */
template<typename TData>
struct CBacktrackList<TData>::CNode
{
public:
    friend class CBacktrackList<TData>;

    /// Move ctor from entry owner
    explicit CNode(std::unique_ptr<SEntry>&&);

    CNode             (const CNode&) = delete; ///< Rule of 5
    CNode& operator = (const CNode&) = delete; ///< Rule of 5

    CNode             (CNode&&) = default; ///< Rule of 5
    CNode& operator = (CNode&&) = default; ///< Rule of 5

    /// Access owning data
    [[nodiscard]] TData& get() const;

private:
    std::unique_ptr<SEntry> entry_ptr_;
};

//----------------------------------------
// CBacktrackList<TData>::CIterator class
//----------------------------------------

/// Iterator class
/**
 * Implements bidirectional iterator for the CBacktrackList template
 * @see CConstIterator
 */
template<typename TData>
class CBacktrackList<TData>::CIterator
{
public:
    friend class CBacktrackList<TData>;

    using difference_type = std::ptrdiff_t; ///< Traits
    using value_type = TData; ///< Traits
    using iterator_category = std::bidirectional_iterator_tag; ///< Traits

    /// Default ctor
    CIterator() = default;

    /// Ctor from list and entry pointers
    CIterator(CBacktrackList<TData>*, SEntry*);

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

    /// implicit add-constness
    operator CBacktrackList<TData>::CConstIterator () const;

    /// InputIt interface
    [[nodiscard]] friend 
    bool operator == (const CIterator& lhs, const CIterator& rhs)
    {
        return (lhs.list_ptr_ == rhs.list_ptr_) && 
               (lhs.entry_ptr_ == rhs.entry_ptr_);
    }

    /// InputIt interface
    [[nodiscard]] friend 
    bool operator != (const CIterator& lhs, const CIterator& rhs)
    {
        return !(lhs == rhs);
    }

private:
    CBacktrackList<TData>* list_ptr_ = nullptr;
    SEntry* entry_ptr_ = nullptr;
};

//----------------------------------------
// CBacktrackList<TData>::CConstIterator class
//----------------------------------------

/// ConstIterator class
/**
 * Implements constant bidirectional iterator for the CBacktrackList template
 * @see CIterator
 */
template<typename TData>
class CBacktrackList<TData>::CConstIterator
{
public:
    friend class CBacktrackList<TData>;

    using difference_type = std::ptrdiff_t; ///< Traits
    using value_type = const TData; ///< Traits
    using iterator_category = std::bidirectional_iterator_tag; ///< Traits

    /// Default ctor
    CConstIterator() = default;

    /// Ctor from list and entry pointers
    CConstIterator(const CBacktrackList<TData>*, const SEntry*);

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
               (lhs.entry_ptr_ == rhs.entry_ptr_);
    }

    /// InputIt interface
    [[nodiscard]] friend 
    bool operator != (const CConstIterator& lhs, const CConstIterator& rhs)
    {
        return !(lhs == rhs);
    }

private:
    const CBacktrackList<TData>* list_ptr_ = nullptr;
    const SEntry* entry_ptr_ = nullptr;
};

//----------------------------------------
// CBacktrackList<TData>::CNode methods
//----------------------------------------

/**
 * @param [in] entry_ptr unique_ptr to entry for ownership transfer
 */
template<typename TData>
CBacktrackList<TData>::CNode::
CNode(std::unique_ptr<SEntry>&& entry_ptr):
    entry_ptr_(std::move(entry_ptr))
{}

/**
 * @return reference to internal data
 */
template<typename TData>
TData&
CBacktrackList<TData>::CNode::
get() const
{
    return entry_ptr_->data;
}

//----------------------------------------
// CBacktrackList<TData>::CIterator methods
//----------------------------------------

/**
 * @param [in] list_ptr pointer to parent list
 * @param [in] entry_ptr pointer to corresponding entry
 */
template<typename TData> 
CBacktrackList<TData>::CIterator::
CIterator(CBacktrackList<TData>* list_ptr, 
          SEntry* entry_ptr):
    list_ptr_{ list_ptr },
    entry_ptr_{ entry_ptr }
{}

/**
 * @return reference to the corresponding entry's data
 */
template<typename TData> 
TData&
CBacktrackList<TData>::CIterator::
operator * () const
{
    return entry_ptr_->data;
}

/**
 * @return pointer to the corresponding entry's data
 */
template<typename TData> 
TData*
CBacktrackList<TData>::CIterator::
operator -> () const
{
    return &(*(*this));
}

/**
 * @return reference to *this
 */
template<typename TData> 
CBacktrackList<TData>::CIterator&
CBacktrackList<TData>::CIterator::
operator ++ ()
{
    entry_ptr_ = entry_ptr_->next.get();
    return *this;
}

/**
 * @return reference to *this
 */
template<typename TData> 
CBacktrackList<TData>::CIterator&
CBacktrackList<TData>::CIterator::
operator -- ()
{
    entry_ptr_ = (entry_ptr_ ? entry_ptr_->prev : list_ptr_->tail_);
    return *this;
}

/**
 * @return old *this
 */
template<typename TData> 
const CBacktrackList<TData>::CIterator
CBacktrackList<TData>::CIterator::
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
const CBacktrackList<TData>::CIterator
CBacktrackList<TData>::CIterator::
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
CBacktrackList<TData>::CIterator::
operator CBacktrackList<TData>::CConstIterator () const
{
    return CConstIterator(list_ptr_, entry_ptr_);
}

//----------------------------------------
// CBacktrackList<TData>::CConstIterator methods
//----------------------------------------

/**
 * @param [in] list_ptr pointer to parent list
 * @param [in] entry_ptr pointer to corresponding entry
 */
template<typename TData> 
CBacktrackList<TData>::CConstIterator::
CConstIterator(const CBacktrackList<TData>* list_ptr, 
               const SEntry* entry_ptr):
    list_ptr_{ list_ptr },
    entry_ptr_{ entry_ptr }
{}

/**
 * @return reference to the corresponding entry's data
 */
template<typename TData> 
const TData&
CBacktrackList<TData>::CConstIterator::
operator * () const
{
    return entry_ptr_->data;
}

/**
 * @return pointer to the corresponding entry's data
 */
template<typename TData> 
const TData*
CBacktrackList<TData>::CConstIterator::
operator -> () const
{
    return &(*(*this));
}

/**
 * @return reference to *this
 */
template<typename TData> 
CBacktrackList<TData>::CConstIterator&
CBacktrackList<TData>::CConstIterator::
operator ++ ()
{
    entry_ptr_ = entry_ptr_->next.get();
    return *this;
}

/**
 * @return reference to *this
 */
template<typename TData> 
CBacktrackList<TData>::CConstIterator&
CBacktrackList<TData>::CConstIterator::
operator -- ()
{
    entry_ptr_ = (entry_ptr_ ? entry_ptr_->prev : list_ptr_->tail_);
    return *this;
}

/**
 * @return old *this
 */
template<typename TData> 
const CBacktrackList<TData>::CConstIterator
CBacktrackList<TData>::CConstIterator::
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
const CBacktrackList<TData>::CConstIterator
CBacktrackList<TData>::CConstIterator::
operator -- (int)
{
    const auto that = *this;
    --(*this);

    return that;
}

//----------------------------------------
// CBacktrackList<TData> methods
//----------------------------------------

/**
 * @param [in] begin_it range begin iterator
 * @param [in] end_it range end iterator
 */
template<typename TData>
template<typename TIter>
CBacktrackList<TData>::
CBacktrackList(TIter&& begin_it, TIter&& end_it)
{
    auto it = std::forward<TIter>(begin_it);
    if (it != end_it)
    {
        head_ = std::make_unique<SEntry>(
                SEntry { .data = *it, .next = {}, .prev = nullptr });
        tail_ = head_.get();
        ++it, ++size_;
    }

    for (; it != std::forward<TIter>(end_it); ++it, ++size_)
    {
        tail_->next = std::make_unique<SEntry>(
                SEntry { .data = *it, .next = {}, .prev = tail_ });
        tail_ = tail_->next.get();
    }
}

/**
 * @param [in] iter iterator to the entry
 * @return node owning the entry
 */
template<typename TData> 
CBacktrackList<TData>::CNode 
CBacktrackList<TData>::extract(const CIterator iter)
{
    std::unique_ptr<SEntry> node_ptr;
    auto entry = iter.entry_ptr_;

    if (entry->next)
        entry->next->prev = entry->prev;
    else
        tail_ = entry->prev;
    
    if (entry->prev)
    {
        node_ptr = std::move(entry->prev->next);
        entry->prev->next = std::move(entry->next);
    }
    else
    {
        node_ptr = std::move(head_);
        head_ = std::move(entry->next);
    }

    entry->next.reset();

    --size_;

    return CNode(std::move(node_ptr));
}

/**
 * @param [in] node node owning the entry
 * @return iterator to the entry
 */
template<typename TData> 
CBacktrackList<TData>::CIterator 
CBacktrackList<TData>::restore(CNode&& node)
{
    CIterator result(this, node.entry_ptr_.get());

    if (node.entry_ptr_->prev)
    {
        node.entry_ptr_->next = std::move(node.entry_ptr_->prev->next);
        node.entry_ptr_->prev->next = std::move(node.entry_ptr_);
    }
    else
    {
        node.entry_ptr_->next = std::move(head_);
        head_ = std::move(node.entry_ptr_);
    }

    ++size_;

    return result;
}

/**
 * @param [in] data data copied to the new entry that is pushed back
 * @return iterator to the new entry
 */
template<typename TData> 
CBacktrackList<TData>::CIterator 
CBacktrackList<TData>::
push_back(const TData& data)
{
    if (tail_)
    {
        tail_->next = std::make_unique<SEntry>(
                SEntry { .data = data, 
                .next = {}, .prev = tail_ });
        tail_ = tail_->next.get();
    }
    else
    {
        head_ = std::make_unique<SEntry>(
                SEntry { .data = data, 
                .next = {}, .prev = nullptr });
        tail_ = head_.get();
    }

    ++size_;

    return CIterator(this, tail_);
}

/**
 * @param [in] data data moved to the new entry that is pushed back
 * @return iterator to the new entry
 */
template<typename TData> 
CBacktrackList<TData>::CIterator 
CBacktrackList<TData>::
push_back(TData&& data)
{
    if (tail_)
    {
        tail_->next = std::make_unique<SEntry>(
                SEntry { .data = std::move(data), 
                .next = {}, .prev = tail_ });
        tail_ = tail_->next.get();
    }
    else
    {
        head_ = std::make_unique<SEntry>(
                SEntry { .data = std::move(data), 
                .next = {}, .prev = nullptr });
        tail_ = head_.get();
    }

    ++size_;

    return CIterator(this, tail_);
}

} // namespace tinysat

#endif // TINYSAT_CBACKTRACKLIST_HPP_
