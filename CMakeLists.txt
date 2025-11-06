# Copyright (C) 2025  Aleksa Radomirovic
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.31)
project(errstr LANGUAGES C)

set(CMAKE_C_STANDARD 23)

option(BUILD_SHARED_LIBS "Build as shared library" ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

add_library(
    errstr
        src/errstr.c
)
target_include_directories(
    errstr
    PUBLIC
        include
)
target_compile_definitions(
    errstr
    PRIVATE
        $<$<BOOL:${ERRSTR_DEFAULT_BUF_SIZE}>:ERRSTR_DEFAULT_BUF_SIZE=${ERRSTR_DEFAULT_BUF_SIZE}>
        $<$<BOOL:${ERRSTR_MAX_BUF_SIZE}>:ERRSTR_MAX_BUF_SIZE=${ERRSTR_MAX_BUF_SIZE}>
)

if(BUILD_TESTING AND PROJECT_IS_TOP_LEVEL)
    include(CTest)
    enable_language(CXX)
    find_package(GTest REQUIRED)

    add_executable(
        errstrftest
            test/errstrftest.cpp
    )
    target_link_libraries(
        errstrftest
            errstr
            GTest::gtest
            GTest::gtest_main
    )

    gtest_discover_tests(errstrftest)
endif()

if(GENERATE_DOCS AND PROJECT_IS_TOP_LEVEL)
    find_package(Doxygen REQUIRED)

    set(DOXYGEN_FILE_PATTERNS *.h)
    set(DOXYGEN_EXTRACT_ALL YES)

    doxygen_add_docs(
        errstr_docs
        ${CMAKE_SOURCE_DIR}/include
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/include
    )
endif()
