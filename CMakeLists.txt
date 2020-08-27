cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(tinySAT VERSION 2.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(THIRD_PARTY_INSTALL_DIR ${CMAKE_SOURCE_DIR}/third-party)

set(SOURCES 
    src/main.cpp
	src/CBinarySolver.cpp
	src/CGeneralSolver.cpp
    src/CDpllSolver.cpp
    )

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/dpll
    ${CMAKE_SOURCE_DIR}/dpll/include
    ${THIRD_PARTY_INSTALL_DIR}/include
    )

link_directories(
    ${CMAKE_SOURCE_DIR}/lib
    ${THIRD_PARTY_INSTALL_DIR}/lib
    )

add_executable(tinySAT ${SOURCES})

# Third-party libraries
add_subdirectory(third-party)

# DPLL algo core static library
add_subdirectory(dpll)

# Tests generated with GTest
add_subdirectory(test)

# Docs generated with Doxygen
add_subdirectory(docs)
add_custom_target(docs DEPENDS tinySAT-docs dpll-docs)

# Tags generated with CTags
add_subdirectory(tags)
add_custom_target(tags DEPENDS tinySAT-tags dpll-tags)

target_link_libraries(tinySAT dpll spdlog)

install(TARGETS tinySAT
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    OBJECTS DESTINATION obj
    )
