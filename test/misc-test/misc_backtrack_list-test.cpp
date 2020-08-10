#include <vector>

#include "misc/CBacktrackSkiplist.hpp"
#include "misc/CBacktrackList.hpp"

#include "gtest/gtest.h"

using namespace tinysat;

TEST(MiscBacktrackTest, list)
{
    std::vector vec = { 1, 2, 3};
    CBacktrackList<int> bl(std::begin(vec), std::end(vec));

    auto bl_it = ++std::begin(bl);
    ASSERT_EQ(*bl_it, 2);

    auto bl_node = bl.extract(bl_it);
    ASSERT_EQ(bl_node.get(), 2);

    bl.push_back(4);
    ASSERT_EQ(bl.size(), 3);

    bl.restore(std::move(bl_node));
    ASSERT_EQ(bl.size(), 4);
}

TEST(MiscBacktrackTest, skiplist)
{
    std::vector vec = { 1, 2, 3};
    CBacktrackSkiplist bsl(std::move(vec));

    auto bsl_it = bsl.find(2);
    ASSERT_EQ(*bsl_it, 2);

    auto bsl_node = bsl.extract(bsl_it);
    ASSERT_EQ(bsl_node.get(), 2);
    ASSERT_EQ(bsl.size(), 2);

    bsl.restore(std::move(bsl_node));
    ASSERT_EQ(bsl.size(), 3);
}
