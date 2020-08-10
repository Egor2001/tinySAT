cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

find_package(Threads)

add_subdirectory(misc-test)
add_subdirectory(dpll-test)
add_subdirectory(solver-test)
