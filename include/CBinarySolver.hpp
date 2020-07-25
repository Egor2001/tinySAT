#ifndef TINYSAT_CBINARYSOLVER_HPP_
#define TINYSAT_CBINARYSOLVER_HPP_

#include <vector>
#include <memory>

#include "CMatchIterator.hpp"
#include "CException.hpp"
#include "SFormula.hpp"

namespace tinysat {

// SAT solver implementing a 2-SAT algorithm
//
// TODO:
// - To search for multiple solutions
// - To imlement LegasySolver concept
//
class CBinarySolver
{
public:
    class CContext;

    using context_t = CContext;
    using CIterator = CMatchIterator<const CBinarySolver>;

    explicit CBinarySolver(const SFormula&);
    explicit CBinarySolver(SFormula&&);

    CBinarySolver             (CBinarySolver&&) = default;
    CBinarySolver& operator = (CBinarySolver&&) = default;

    void reset(const SFormula&);
    void reset(SFormula&&);

    [[nodiscard]] std::unique_ptr<CContext> context() const;
    void proceed(std::unique_ptr<CContext>&) const;

    [[nodiscard]] CIterator begin() const;
    [[nodiscard]] CIterator end() const;

protected:
    void init();

private:
    SFormula formula_;

    std::vector<size_t> comp_vec_;
    std::vector<std::vector<size_t>> cond_edge_vec_;
};

// Context incapsulating data for the CBinarySolver algorithm
//
// TODO:
// - To imlement LegasyContext concept
//
class CBinarySolver::CContext
{
public:
    CContext             (const CContext&) = default;
    CContext& operator = (const CContext&) = default;

    CContext             (CContext&&) = default;
    CContext& operator = (CContext&&) = default;

    [[nodiscard]] SMatch match() const;

    friend bool operator == (const CContext& lhs, const CContext& rhs);
    friend bool operator != (const CContext& lhs, const CContext& rhs);

protected:
    CContext(const std::vector<size_t>& order_vec);
    CContext(std::vector<size_t>&& order_vec);

private:
    std::vector<size_t> order_vec_;
};

} // namespace tinysat 

#endif // TINYSAT_CBINARYSOLVER_HPP_
