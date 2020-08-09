#ifndef TINYSAT_CMATCHITERATOR_HPP_
#define TINYSAT_CMATCHITERATOR_HPP_

#include <memory>

#include "SFormula.hpp"

namespace tinysat {

// Template interface over solver context
//
// Satisfies the LegasyIterator concept
//
// Requirements:
// - TSolver satisfies Solver requirements
// - TContext satisfies Context requirements and is compatible with TSolver
//
// TODO: 
// - To satisfy the LegasyInputIterator concept
// - To implement iterator_traits
//
template<typename TSolver, typename TContext = TSolver::context_t>
class CMatchIterator
{
public:
    CMatchIterator(TSolver* solver, std::unique_ptr<TContext>&& context):
        solver_(solver),
        context_(std::move(context))
    {}

    CMatchIterator(const CMatchIterator& other):
        solver_(other.solver_),
        context_(other.context_ == nullptr ? 
                 nullptr : std::make_unique<TContext>(*other.context_))
    {}

    CMatchIterator& operator = (const CMatchIterator& other)
    {
        if (this == &other)
            return *this;

        solver_ = other.solver_;
        context_.reset(other.context_ == nullptr ? 
                       nullptr : std::make_unique<TContext>(*other.context_));

        return *this;
    }

    CMatchIterator             (CMatchIterator&&) = default;
    CMatchIterator& operator = (CMatchIterator&&) = default;

    SMatch operator * () const
    {
        return context_->match();
    }

    CMatchIterator& operator ++ ()
    {
        solver_->proceed(context_);
        return *this;
    }

    friend bool operator == (const CMatchIterator& lhs, 
                             const CMatchIterator& rhs)
    {
        return (lhs.solver_ == rhs.solver_) && 
            (lhs.context_ == rhs.context_ || (lhs.context_ && rhs.context_ && 
                                              *lhs.context_ == *rhs.context_));
    }

    friend bool operator != (const CMatchIterator& lhs, 
                             const CMatchIterator& rhs)
    {
        return !(lhs == rhs);
    }

private:
    TSolver* solver_;
    std::unique_ptr<TContext> context_;
};

} // namespace tinysat

#endif // TINYSAT_CMATCHITERATOR_HPP_
