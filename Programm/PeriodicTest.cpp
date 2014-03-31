// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "PeriodicTest.hpp"

#include "Periodic.hpp"

void PeriodicTest::test_periodic() {
    CPPUNIT_ASSERT(Periodic::wrap(0, 1) == 0);
    CPPUNIT_ASSERT(Periodic::wrap(1, 1) == 0);

    CPPUNIT_ASSERT(Periodic::wrap(0, 2) == 0);
    CPPUNIT_ASSERT(Periodic::wrap(1, 2) == 1);
    CPPUNIT_ASSERT(Periodic::wrap(2, 2) == 0);

    CPPUNIT_ASSERT(Periodic::wrap(10, 10) == 0);
}

void PeriodicTest::test_periodic_negative() {
    CPPUNIT_ASSERT(Periodic::wrap(-1, 1) == 0);
    CPPUNIT_ASSERT(Periodic::wrap(-1, 2) == 1);

    CPPUNIT_ASSERT(Periodic::wrap(-1, 10) == 9);
    CPPUNIT_ASSERT(Periodic::wrap(-2, 10) == 8);
}

void PeriodicTest::test_prevent_sign_flip() {
    CPPUNIT_ASSERT(Periodic::wrap(-1, 10) != 1);
    CPPUNIT_ASSERT(Periodic::wrap(-2, 10) != 2);
}
