# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

cmake_minimum_required(VERSION 2.6)
project(Bachelorarbeit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pedantic -std=c++11 -Wall")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -Wall")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

add_executable(metropolis
    main-metropolis.cpp

    BoostHistogram.cpp
    BootstrappedHistogram.cpp
    BootstrappedQuantity.cpp
    BootstrapPool.cpp
    BootstrapSample.cpp
    Computable.cpp
    FixedHistogram.cpp
    HarmonicOscillator.cpp
    Histogram.cpp
    ListQuantity.cpp
    MetropolisAlgorithm.cpp
    MetropolisDriver.cpp
    Moment.cpp
    parse_arguments.cpp
    Periodic.cpp
    ResultSet.cpp
    Settings.cpp
    SizePrinter.cpp
    System.cpp
    Trajectory.cpp
    VectorHistogram.cpp

    )
target_link_libraries(metropolis boost_program_options)

add_executable(hello EXCLUDE_FROM_ALL

    main-hello.cpp)
target_link_libraries(hello boost_program_options)

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

add_executable(test-distribution EXCLUDE_FROM_ALL
    main-test-distribution.cpp

    BoostHistogram.cpp
    Histogram.cpp
    SizePrinter.cpp
    VectorHistogram.cpp
    )

add_executable(action-difference EXCLUDE_FROM_ALL
    main-action-difference.cpp

    HarmonicOscillator.cpp
    Settings.cpp
    System.cpp
    )
