// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

/**
  @file
  */

#ifndef PERIODICTEST_H
#define PERIODICTEST_H

#include <cppunit/extensions/HelperMacros.h>

class PeriodicTest: public CppUnit::TestFixture {
        CPPUNIT_TEST_SUITE(PeriodicTest);
        CPPUNIT_TEST(test_periodic);
        CPPUNIT_TEST(test_periodic_negative);
        CPPUNIT_TEST(test_prevent_sign_flip);
        CPPUNIT_TEST_SUITE_END();

    public:
        void test_periodic();
        void test_periodic_negative();
        void test_prevent_sign_flip();
};

#endif /* end of include guard: PERIODICTEST_H */
