# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

cmake_minimum_required(VERSION 2.6)
project(Bachelorarbeit C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3 -pthread -std=c++11 -Wall -Werror")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

find_package(Threads)

add_executable(metropolis
    main-metropolis.cpp

    AnharmonicOscillator.cpp
    BootstrappedHistogram.cpp
    BootstrappedQuantity.cpp
    BootstrapPool.cpp
    BootstrapSample.cpp
    Computable.cpp
    ConcurrentCounter.cpp
    Correlation.cpp
    FixedHistogram.cpp
    HarmonicOscillator.cpp
    Histogram.cpp
    HistogramComputable.cpp
    ListQuantity.cpp
    MetropolisAlgorithm.cpp
    MetropolisDriver.cpp
    Moment.cpp
    parse_arguments.cpp
    Periodic.cpp
    PositionDensity.cpp
    ResultSet.cpp
    Settings.cpp
    SizePrinter.cpp
    System.cpp
    Trajectory.cpp
    VectorHistogram.cpp

    )
target_link_libraries(metropolis boost_program_options)
target_link_libraries(metropolis boost_system)
target_link_libraries(metropolis boost_filesystem)
target_link_libraries (metropolis ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(metropolis crypto++)

add_executable(test-metropolis EXCLUDE_FROM_ALL
    main-metropolis-test.cpp

    HarmonicOscillator.cpp
    HarmonicOscillatorTest.cpp
    MetropolisAlgorithm.cpp
    MetropolisAlgorithmTest.cpp
    Periodic.cpp
    PeriodicTest.cpp
    System.cpp
    )
target_link_libraries(test-metropolis cppunit)
