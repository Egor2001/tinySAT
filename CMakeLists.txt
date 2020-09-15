cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(tinySAT VERSION 2.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O0 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Ofast")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(THIRD_PARTY_INSTALL_DIR ${CMAKE_SOURCE_DIR}/third-party)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(TINYSAT_AS_ROOT ON)
else()
    set(TINYSAT_AS_ROOT OFF)
endif()

set(SOURCES 
    src/CBinarySolver.cpp
    src/CGeneralSolver.cpp
    src/CDpllSolver.cpp
    )

if (TINYSAT_AS_ROOT)
    set(SOURCES 
        src/main.cpp
        ${SOURCES}
        )
endif()

include_directories(
    ${tinySAT_SOURCE_DIR}/include
    ${tinySAT_SOURCE_DIR}/include/core
    ${tinySAT_SOURCE_DIR}/dpll
    ${tinySAT_SOURCE_DIR}/dpll/include
    ${THIRD_PARTY_INSTALL_DIR}/include
    )

link_directories(
    ${tinySAT_SOURCE_DIR}/lib
    ${THIRD_PARTY_INSTALL_DIR}/lib
    )

if (TINYSAT_AS_ROOT)
    add_executable(tinySAT ${SOURCES})
else()
    add_library(tinySAT SHARED ${SOURCES})
endif()

# Third-party libraries
add_subdirectory(third-party)

# DPLL algo core static library
add_subdirectory(dpll)

if (TINYSAT_AS_ROOT)
# Tests generated with GTest
    add_subdirectory(test)

# Docs generated with Doxygen
    add_subdirectory(docs)
    add_custom_target(docs DEPENDS tinySAT-docs dpll-docs)

# Tags generated with CTags
    add_subdirectory(tags)
    add_custom_target(tags DEPENDS tinySAT-tags dpll-tags)
endif()

target_link_libraries(tinySAT dpll spdlog)

if(TINYSAT_AS_ROOT)
    # TODO: to replace with CMAKE_INSTALL_LIBDIR/CMAKE_INSTALL_BINDIR
    install(TARGETS tinySAT
        RUNTIME DESTINATION ${TINYSAT_INSTALL_DIR}/bin
        LIBRARY DESTINATION lib
        OBJECTS DESTINATION obj
        )
else()
    # TODO: to replace with CMAKE_INSTALL_LIBDIR
    install(TARGETS tinySAT
        LIBRARY DESTINATION ${CMAKE_INSTALL_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_DIR}/lib
        )

    # TODO: to replace with CMAKE_INSTALL_INCLUDEDIR
    install(DIRECTORY include/ 
        DESTINATION ${CMAKE_INSTALL_DIR}/include
        )
endif()
