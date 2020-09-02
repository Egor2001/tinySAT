#include <iostream>

#include "CDpllSolver.hpp"

#include "gtest/gtest.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

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

    auto solver = CDpllSolver(formula, spdlog::stdout_color_mt("console"));
    auto it = solver.begin(), end = solver.end();

    ASSERT_NE(it, end);
    ASSERT_TRUE(formula.is_match(*it));
}
