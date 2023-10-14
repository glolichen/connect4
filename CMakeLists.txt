cmake_minimum_required(VERSION 3.0.0)
project(connect4 VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} --debug -g -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wextra -pedantic")

add_executable(connect4 bitboard.cpp game.cpp move.cpp hash.cpp search.cpp main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
