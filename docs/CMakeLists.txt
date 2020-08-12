cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

find_package(Doxygen)

if(DOXYGEN_FOUND EQUAL FALSE)
    message("error: can't find Doxygen")
    return()
endif()

doxygen_add_docs(tinySAT-docs
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tinySAT
    )

doxygen_add_docs(dpll-docs
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/dpll
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dpll
    )
