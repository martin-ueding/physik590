# Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
# Licensed under The GNU Public License Version 2 (or later)

cmake_minimum_required(VERSION 2.6)
project(Bachelorarbeit C CXX)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -Wall -Wextra -Wpedantic")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

add_definitions("-march=native")
add_definitions("-pthread")
add_definitions("-std=c++11")
add_definitions("-Werror")
add_definitions("-Wall")
add_definitions("-Wdocumentation")

find_package(Boost COMPONENTS filesystem program_options serialization system REQUIRED)
find_package(Threads REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(metropolis
    main-metropolis.cpp

    Analysis.cpp
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
    SizePrinter.cpp
    VectorHelper.cpp

    )

add_executable(metropolis-test
    main-metropolis-test.cpp

    Settings.cpp
    SettingsTest.cpp
    SizePrinter.cpp

    )

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(metropolis ${Boost_LIBRARIES})
target_link_libraries(metropolis ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(metropolis crypto++)
target_link_libraries(metropolis jsoncpp)

target_link_libraries(metropolis-test ${Boost_LIBRARIES})
target_link_libraries(metropolis-test cppunit)
target_link_libraries(metropolis-test crypto++)
target_link_libraries(metropolis-test jsoncpp)

install(TARGETS metropolis DESTINATION bin)
install(PROGRAMS

    metropolis-3d-meta
    metropolis-build-env-set-up
    metropolis-energy-differences
    metropolis-index
    metropolis-metaplot
    metropolis-plotall
    metropolis-sweep

    DESTINATION bin)
