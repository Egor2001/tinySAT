#include <iostream>

#include "CBacktrackSkiplist.hpp"
#include "CBacktrackList.hpp"

using namespace tinysat;

int main()
{
    std::vector vec = { 1, 2, 3};
    CBacktrackList<int> bl(std::begin(vec), std::end(vec));

    auto bl_it = ++std::begin(bl);
    auto bl_node = bl.extract(bl_it);

    for (const auto& num : bl)
        std::cout << num << '\n';

    bl.push_back(4);

    for (const auto& num : bl)
        std::cout << num << '\n';

    bl.restore(std::move(bl_node));

    for (const auto& num : bl)
        std::cout << num << '\n';

    CBacktrackSkiplist bsl(std::move(vec));

    auto bsl_it = bsl.find(2);
    auto bsl_node = bsl.extract(bsl_it);

    for (const auto& num : bsl)
        std::cout << num << '\n';

    bsl.restore(std::move(bsl_node));

    for (const auto& num : bsl)
        std::cout << num << '\n';

    return 0;
}
