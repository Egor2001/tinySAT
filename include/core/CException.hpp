#ifndef TINYSAT_CEXCEPTION_HPP_
#define TINYSAT_CEXCEPTION_HPP_

#include <stdexcept>
#include <string>

namespace tinysat {

class CException : public std::runtime_error
{
public:
    explicit CException(const std::string& what):
        std::runtime_error(what)
    {}

    explicit CException(const char* what):
        std::runtime_error(what)
    {}
};

} // namespace tinysat

#endif // TINYSAT_CEXCEPTION_HPP_
