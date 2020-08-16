cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

include(ExternalProject)

ExternalProject_Add(spdlog_git
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.x
    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}
    -DCMAKE_BUILD_TYPE=Release
    )

ExternalProject_Add(gtest_git 
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG master
    CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}
    -DCMAKE_BUILD_TYPE=Release
    )
