#include <iostream>

#include "CDpllSolver.hpp"

#include "gtest/gtest.h"

using namespace tinysat;

TEST(DpllSolverTest, proceed)
{
    SFormula formula = {
        .params_cnt = 5u,
        .clause_vec = {
            { -1, 2, 5 },
            { 1, 3 },
            { 2, 5 },
            { -3, 4, -5 },
            { -1 },
        }
    };

    auto solver = CDpllSolver(formula);
    auto it = solver.begin(), end = solver.end();

    ASSERT_NE(it, end);
    ASSERT_TRUE(formula.is_match(*it));
}
