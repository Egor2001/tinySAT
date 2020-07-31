#ifndef TINYSAT_CBACKTRACKLIST_HPP_
#define TINYSAT_CBACKTRACKLIST_HPP_

#include <memory>
#include <iterator>

namespace tinysat {

//----------------------------------------
// CBacktrackList<TData> class
//----------------------------------------
template<typename TData>
class CBacktrackList
{
public:
    class CNode;
    class CIterator;
    class CConstIterator;

    using data_type = TData;
    using node_type = CNode;
    using iterator = CIterator;
    using const_iterator = CConstIterator;

    struct SEntry
    {
        TData data;
        std::unique_ptr<SEntry> next;
        SEntry* prev;
    };

    CBacktrackList() = default;

    template<typename TIter>
    CBacktrackList(TIter&&, TIter&&);

    CBacktrackList             (const CBacktrackList&) = default;
    CBacktrackList& operator = (const CBacktrackList&) = default;
    CBacktrackList             (CBacktrackList&&) = default;
    CBacktrackList& operator = (CBacktrackList&&) = default;

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
        return CIterator(this, head_.get());
    }

    [[nodiscard]] CConstIterator begin() const
    {
        return CConstIterator(this, head_.get());
    }

    [[nodiscard]] CConstIterator cbegin() const
    {
        return begin();
    }

    [[nodiscard]] CIterator end()
    {
        return CIterator(this, nullptr);
    }

    [[nodiscard]] CConstIterator end() const
    {
        return CConstIterator(this, nullptr);
    }

    [[nodiscard]] CConstIterator cend() const
    {
        return end();
    }

    CNode extract(const CIterator);
    CIterator restore(CNode&&);

    CIterator push_back(const TData&);
    CIterator push_back(TData&&);

private:
    size_t size_ = 0u;
    std::unique_ptr<SEntry> head_;
    SEntry* tail_ = nullptr;
};

//----------------------------------------
// CBacktrackList<TData>::CNode class
//----------------------------------------
template<typename TData>
struct CBacktrackList<TData>::CNode
{
public:
    friend class CBacktrackList<TData>;

    explicit CNode(std::unique_ptr<SEntry>&&);

    CNode             (const CNode&) = delete;
    CNode& operator = (const CNode&) = delete;

    CNode             (CNode&&) = default;
    CNode& operator = (CNode&&) = default;

    [[nodiscard]] TData& get() const;

private:
    std::unique_ptr<SEntry> entry_ptr_;
};

//----------------------------------------
// CBacktrackList<TData>::CIterator class
//----------------------------------------
template<typename TData>
class CBacktrackList<TData>::CIterator
{
public:
    friend class CBacktrackList<TData>;

    using difference_type = std::ptrdiff_t;
    using value_type = TData;
    using iterator_category = std::bidirectional_iterator_tag;

    CIterator() = default;
    CIterator(CBacktrackList<TData>*, SEntry*);

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

    operator CBacktrackList<TData>::CConstIterator () const;

    [[nodiscard]] friend 
    bool operator == (const CIterator& lhs, const CIterator& rhs)
    {
        return (lhs.list_ptr_ == rhs.list_ptr_) && 
               (lhs.entry_ptr_ == rhs.entry_ptr_);
    }

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
template<typename TData>
class CBacktrackList<TData>::CConstIterator
{
public:
    friend class CBacktrackList<TData>;

    using difference_type = std::ptrdiff_t;
    using value_type = const TData;
    using iterator_category = std::bidirectional_iterator_tag;

    CConstIterator() = default;
    CConstIterator(const CBacktrackList<TData>*, const SEntry*);

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
               (lhs.entry_ptr_ == rhs.entry_ptr_);
    }

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
template<typename TData>
CBacktrackList<TData>::CNode::
CNode(std::unique_ptr<SEntry>&& entry_ptr):
    entry_ptr_(std::move(entry_ptr))
{}

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
template<typename TData> 
CBacktrackList<TData>::CIterator::
CIterator(CBacktrackList<TData>* list_ptr, 
          SEntry* entry_ptr):
    list_ptr_{ list_ptr },
    entry_ptr_{ entry_ptr }
{}

template<typename TData> 
TData&
CBacktrackList<TData>::CIterator::
operator * () const
{
    return entry_ptr_->data;
}

template<typename TData> 
TData*
CBacktrackList<TData>::CIterator::
operator -> () const
{
    return &(*(*this));
}

template<typename TData> 
CBacktrackList<TData>::CIterator&
CBacktrackList<TData>::CIterator::
operator ++ ()
{
    entry_ptr_ = entry_ptr_->next.get();
    return *this;
}

template<typename TData> 
CBacktrackList<TData>::CIterator&
CBacktrackList<TData>::CIterator::
operator -- ()
{
    entry_ptr_ = (entry_ptr_ ? entry_ptr_->prev : list_ptr_->tail_);
    return *this;
}

template<typename TData> 
const CBacktrackList<TData>::CIterator
CBacktrackList<TData>::CIterator::
operator ++ (int)
{
    const auto that = *this;
    ++(*this);

    return that;
}

template<typename TData> 
const CBacktrackList<TData>::CIterator
CBacktrackList<TData>::CIterator::
operator -- (int)
{
    const auto that = *this;
    --(*this);

    return that;
}

template<typename TData> 
CBacktrackList<TData>::CIterator::
operator CBacktrackList<TData>::CConstIterator () const
{
    return CConstIterator(list_ptr_, entry_ptr_);
}

//----------------------------------------
// CBacktrackList<TData>::CConstIterator methods
//----------------------------------------
template<typename TData> 
CBacktrackList<TData>::CConstIterator::
CConstIterator(const CBacktrackList<TData>* list_ptr, 
               const SEntry* entry_ptr):
    list_ptr_{ list_ptr },
    entry_ptr_{ entry_ptr }
{}

template<typename TData> 
const TData&
CBacktrackList<TData>::CConstIterator::
operator * () const
{
    return entry_ptr_->data;
}

template<typename TData> 
const TData*
CBacktrackList<TData>::CConstIterator::
operator -> () const
{
    return &(*(*this));
}

template<typename TData> 
CBacktrackList<TData>::CConstIterator&
CBacktrackList<TData>::CConstIterator::
operator ++ ()
{
    entry_ptr_ = entry_ptr_->next.get();
    return *this;
}

template<typename TData> 
CBacktrackList<TData>::CConstIterator&
CBacktrackList<TData>::CConstIterator::
operator -- ()
{
    entry_ptr_ = (entry_ptr_ ? entry_ptr_->prev : list_ptr_->tail_);
    return *this;
}

template<typename TData> 
const CBacktrackList<TData>::CConstIterator
CBacktrackList<TData>::CConstIterator::
operator ++ (int)
{
    const auto that = *this;
    ++(*this);

    return that;
}

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

    return CNode(std::move(node_ptr));
}

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

    return result;
}

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

    return CIterator(this, tail_);
}

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

    return CIterator(this, tail_);
}

} // namespace tinysat

#endif // TINYSAT_CBACKTRACKLIST_HPP_
