cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(dpll VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Ofast")

set(SOURCES 
    src/CDpllAssignment.cpp
    src/CDpllFormula.cpp
    src/CDpllSortHeap.cpp
    src/CDpllContext.cpp
    )

add_library(dpll ${SOURCES})

target_include_directories(dpll
    PUBLIC ${PROJECT_SOURCE_DIR}/
    PUBLIC ${PROJECT_SOURCE_DIR}/include/
    )
