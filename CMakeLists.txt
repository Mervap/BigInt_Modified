cmake_minimum_required(VERSION 2.8.4)

project(BingInt_Modified)

include_directories(${BIGINT_SOURCE_DIR})

add_executable(myBigInt_mod
        big_integer.h
        big_integer.cpp
        big_integer_testing.cpp
        gtest/gtest-all.cc
        gtest/gtest.h
        gtest/gtest_main.cc
        my_vector.cpp
        my_vector.h)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x -pedantic -O2")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif ()

