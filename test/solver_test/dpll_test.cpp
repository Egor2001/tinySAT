#include <iostream>

#include "CDpllSolver.hpp"

#include "gtest/gtest.h"

using namespace tinysat;

TEST(DpllTest, init)
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

    CDpllContext context(formula);
    ASSERT_EQ(context.init(), true);
    /*
    ASSERT_EQ(context.match(), SMatch{ {...} });
    ASSERT_EQ(context.next(), true);
    ASSERT_EQ(context.match(), SMatch{ {...} });
    */
}
