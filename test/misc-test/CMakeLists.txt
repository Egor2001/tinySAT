cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(misc_test)

add_executable(misc_test misc_backtrack_list-test.cpp)

target_link_libraries(misc_test 
    gtest gtest_main Threads::Threads
    )
