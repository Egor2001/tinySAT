#include "CDpllSolver.hpp"

/**
 * @file
 * @author geome_try
 * @date 2020
 */

/// @brief
namespace tinysat {

/**
 * @param [in] formula SAT formula to init context
 * @param [in] logger shared spdlog logger
 */
CDpllSolver::CContext::CContext(const SFormula& formula,
        std::shared_ptr<spdlog::logger> logger):
    dpll_context_(formula),
    logger_(std::move(logger))
{
    SPDLOG_LOGGER_INFO(logger_, "[ctor] CDpllSolver::CContext() [this = {}]", 
            static_cast<const void*>(this));
}

/**
 * @return current match object
 */
SMatch CDpllSolver::CContext::match() const
{
    SPDLOG_LOGGER_INFO(logger_, "CDpllSolver::CContext::match()");

    return dpll_context_.match();
}

/**
 * @param [in] formula SAT formula to copy from
 * @param [in] logger shared spdlog logger
 */
CDpllSolver::CDpllSolver(const SFormula& formula,
        std::shared_ptr<spdlog::logger> logger):
    formula_(formula),
    logger_(std::move(logger))
{
    SPDLOG_LOGGER_INFO(logger_, "[ctor] CDpllSolver() [this = {}]", 
            static_cast<const void*>(this));
}

/**
 * @param [in] formula SAT formula to move from
 * @param [in] logger shared spdlog logger
 */
CDpllSolver::CDpllSolver(SFormula&& formula,
        std::shared_ptr<spdlog::logger> logger):
    formula_(std::move(formula)),
    logger_(std::move(logger))
{
    SPDLOG_LOGGER_INFO(logger_, "[ctor] CDpllSolver() [this = {}]", 
            static_cast<const void*>(this));
}

/**
 * @return unique pointer to the constructed context
 * @see proceed()
 */
std::unique_ptr<CDpllSolver::CContext> CDpllSolver::context() const
{
    SPDLOG_LOGGER_INFO(logger_, "CDpllSolver::context()");

    auto context = std::make_unique<CContext>(formula_, logger_);
    if (context->dpll_context_.init() == false)
        context.reset();

    return context;
}

/**
 * @param [in, out] unique pointer to a context
 * @see context()
 */
void CDpllSolver::proceed(std::unique_ptr<CContext>& context) const
{
    SPDLOG_LOGGER_INFO(logger_, "CDpllSolver::proceed()");

    if (context == nullptr)
        throw CException("trying to proceed over the end");

    if (context->dpll_context_.next() == false)
        context.reset();
}

/**
 * @return iterator to the first solution
 * @see end()
 */
CDpllSolver::CIterator CDpllSolver::begin() const
{
    SPDLOG_LOGGER_INFO(logger_, "CDpllSolver::begin()");

    return CIterator(this, context());
}

/**
 * @return iterator past the last solution
 * @see begin()
 */
CDpllSolver::CIterator CDpllSolver::end() const
{
    SPDLOG_LOGGER_INFO(logger_, "CDpllSolver::end()");

    return CIterator(this, nullptr);
}

/**
 * @param [in] lhs one context object
 * @param [in] rhs another context object
 * @return true if objects are equal
 * @see operator!=()
 */
bool operator == (const CDpllSolver::CContext& lhs,
                  const CDpllSolver::CContext& rhs)
{
    return lhs.dpll_context_ == rhs.dpll_context_;
}

/**
 * @param [in] lhs one context object
 * @param [in] rhs another context object
 * @return true if objects aren't equal
 * @see operator==()
 */
bool operator != (const CDpllSolver::CContext& lhs,
                  const CDpllSolver::CContext& rhs)
{
    return !(lhs == rhs);
}

} // namespace tinysat
