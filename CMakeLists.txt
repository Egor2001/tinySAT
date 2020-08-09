cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(tinySAT VERSION 2.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

set(SOURCES 
	src/CBinarySolver.cpp
	src/CGeneralSolver.cpp
    src/CDpllSolver.cpp
    )

add_executable(tinySAT main.cpp ${SOURCES})

target_include_directories(tinySAT 
    PUBLIC ${PROJECT_SOURCE_DIR}/
    PUBLIC ${PROJECT_SOURCE_DIR}/include/
    PUBLIC ${PROJECT_SOURCE_DIR}/include/core
    )

add_subdirectory(include/dpll)
target_link_libraries(tinySAT dpll)

set(CTAGS ctags)
set(TAGS tags)

add_custom_target(tags)
add_custom_command(TARGET tags
    PRE_BUILD
    COMMAND ${CTAGS}
    ARGS -Rf ${TAGS}
    BYPRODUCTS ${TAGS}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

install(TARGETS tinySAT
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    OBJECTS DESTINATION obj
    )
