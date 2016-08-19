cmake_minimum_required(VERSION 2.8)
project(libneedle)
set(CMAKE_BUILD_TYPE Release)

# Warnings / extra flags:
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Weverything -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Weverything -pedantic -Wno-c++98-compat")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic")
    # TODO add other warning checks here too
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /WX /Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX /Wall")
endif()

include_directories(include)
subdirs(src tests)

