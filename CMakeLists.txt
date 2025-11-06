# Copyright (C) 2025  Aleksa Radomirovic
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.31)
project(errstr LANGUAGES C)

set(CMAKE_C_STANDARD 23)

option(BUILD_SHARED_LIBS "Build as shared library" ON)

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
