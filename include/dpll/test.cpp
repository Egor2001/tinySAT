#include <iostream>

#include "CDpllContext.hpp"

using namespace tinysat;

int main()
{
    SFormula formula = {
        .params_cnt = 5u,
        .clause_vec = {
            { -1, 2, 5 },
            { 1, 3 },
            { 2, 5 },
            { -3, 4, -5 },
            { 1 },
//            { -1 },
        }
    };

    CDpllContext context(formula);
    std::cout << context.init();

    return 0;
}
