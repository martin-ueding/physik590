// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#ifndef METROPOLISALGORITHMTEST_H
#define METROPOLISALGORITHMTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "MetropolisAlgorithm.hpp"

class MetropolisAlgorithmTest : public CppUnit::TestFixture {
        CPPUNIT_TEST_SUITE(MetropolisAlgorithmTest);
        CPPUNIT_TEST(test_sign);
        CPPUNIT_TEST_SUITE_END();

    public:
        void test_sign();
};

#endif /* end of include guard: METROPOLISALGORITHMTEST_H */
