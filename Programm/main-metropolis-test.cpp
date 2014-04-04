// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#include <cppunit/TextTestRunner.h>

#include "MetropolisAlgorithmTest.hpp"
#include "PeriodicTest.hpp"

int main() {
    CppUnit::TextTestRunner runner;
    runner.addTest(PeriodicTest::suite());
    runner.addTest(MetropolisAlgorithmTest::suite());
    runner.run();
    return 0;
}

