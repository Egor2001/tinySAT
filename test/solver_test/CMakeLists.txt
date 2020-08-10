cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(solver_test)
find_package(Threads)

add_executable(dpll_test dpll_test.cpp)

target_include_directories(dpll_test
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${CMAKE_SOURCE_DIR}/include/dpll
    PUBLIC ${CMAKE_SOURCE_DIR}/include/core
    )

target_link_directories(dpll_test
    PUBLIC ${CMAKE_SOURCE_DIR}/lib
    )

target_link_libraries(dpll_test 
    dpll
    gtest gtest_main Threads::Threads)
