cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(solver_test)

add_executable(solver_test dpll_solver-test.cpp 
    ${CMAKE_SOURCE_DIR}/src/CDpllSolver.cpp)

target_link_libraries(solver_test 
    dpll
    gtest gtest_main Threads::Threads
    )
