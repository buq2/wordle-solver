cmake_minimum_required(VERSION 3.5)
project(PhotoRePoEst VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

include(cmake/conan_config.cmake)
include(cmake/output_paths.cmake)

find_package(tinyutf8 REQUIRED)

ADD_LIBRARY(wordle
    wordle.cpp
    wordle_solver.cpp)
TARGET_LINK_LIBRARIES(wordle
    tinyutf8::tinyutf8)

ADD_EXECUTABLE(wordle_solver
    main.cpp
    )
TARGET_LINK_LIBRARIES(wordle_solver
    wordle)

ADD_EXECUTABLE(wordle_auto_player
    main_auto_player.cpp
    )
TARGET_LINK_LIBRARIES(wordle_auto_player
    wordle)
