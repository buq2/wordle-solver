cmake_minimum_required(VERSION 3.5)
project(PhotoRePoEst VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

include(cmake/conan_config.cmake)
include(cmake/output_paths.cmake)

find_package(tinyutf8 REQUIRED)

ADD_EXECUTABLE(wordle_solver
    wordle_solver.cpp)
TARGET_LINK_LIBRARIES(wordle_solver
    tinyutf8::tinyutf8)
