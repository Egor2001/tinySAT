#include <iostream>

#include "SFormula.hpp"
#include "CBinarySolver.hpp"
#include "CGeneralSolver.hpp"
#include "CDpllSolver.hpp"

using namespace tinysat;

int main(int argc, const char* argv[])
{
    SFormula formula =
    {
        .params_cnt = 3u,
        .clause_vec = {{1, -2}, {2, -3}, {3, -1}}
    };

    auto bin_solver = CBinarySolver(formula);
    auto gen_solver = CGeneralSolver(formula);
    auto dpll_solver = CDpllSolver(formula);

    std::cout << "binary solver\n";
    for (const auto& match : bin_solver)
        std::cout << match << '\n';

    std::cout << "general solver\n";
    for (const auto& match : gen_solver)
        std::cout << match << '\n';

    std::cout << "dpll solver\n";
    for (const auto& match : dpll_solver)
        std::cout << match << '\n';

    return 0;
}
