#include <iostream>

#include "CSolverContext.hpp"

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
        }
    };

    SDpllFormula dpll_formula(std::move(formula));

    auto state = dpll_formula.get_state();
    auto prop_res = dpll_formula.propagate(-1);
    // auto back_res = dpll_formula.backtrack(state);

    std::cout << "conflict = " << prop_res.conflict << '\n';

    return 0;
}
