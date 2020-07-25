#include "SFormula.hpp"

namespace tinysat {

bool SFormula::is_match(const SMatch& match) const
{
    if (this->params_cnt != match.value_vec.size())
        return false;

    bool result = true;
    for (const auto& clause : this->clause_vec)
    {
        bool clause_result = false;
        for (const auto& literal : clause)
        {
            clause_result = clause_result || 
                (literal < 0 ? 
                 match.value_vec[-literal - 1u] == SMatch::EValue::FALSE :
                 match.value_vec[literal - 1u] == SMatch::EValue::TRUE);

            if (clause_result)
                break;
        }

        result = result && clause_result;

        if (!result)
            break;
    }

    return result;
}

std::ostream& operator << (std::ostream& stream, const SFormula& formula)
{
    stream << "[ ";
    stream << "p " << formula.params_cnt << " ";
    stream << "c " << formula.clause_vec.size() << " ";

    for (const auto& clause : formula.clause_vec)
    {
        stream << "[ ";
        for (const auto& val : clause)
            stream << val << " ";

        stream << "] ";
    }

    stream << "]";

    return stream;
}

} // namespace tinysat
