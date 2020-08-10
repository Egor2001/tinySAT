cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(solver_test)

add_executable(dpll_test dpll_context-test.cpp)

target_link_libraries(dpll_test 
    dpll
    gtest gtest_main Threads::Threads
    )
