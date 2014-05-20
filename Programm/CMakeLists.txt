# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

cmake_minimum_required(VERSION 2.6)
project(Bachelorarbeit C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3 -pthread -std=c++11 -Wall -Werror")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

find_package(Boost COMPONENTS filesystem program_options system REQUIRED)
find_package(Threads REQUIRED)

add_executable(metropolis
    main-metropolis.cpp

    BootstrappedHistogram.cpp
    BootstrappedQuantity.cpp
    BootstrapPool.cpp
    BootstrapSample.cpp
    Computable.cpp
    ConcurrentCounter.cpp
    Correlation.cpp
    GEVPSolver.cpp
    Histogram.cpp
    HistogramComputable.cpp
    MetropolisAlgorithm.cpp
    MetropolisDriver.cpp
    Oscillator.cpp
    parse_arguments.cpp
    Periodic.cpp
    PositionDensity.cpp
    ProgressBar.cpp
    ResultSet.cpp
    Settings.cpp
    VectorHelper.cpp

    )

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(metropolis ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries(metropolis ${Boost_FILESYSTEM_LIBRARY})
target_link_libraries(metropolis ${Boost_SYSTEM_LIBRARY})

target_link_libraries(metropolis ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(metropolis crypto++)
