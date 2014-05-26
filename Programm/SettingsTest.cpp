// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "SettingsTest.hpp"

#include "Settings.hpp"

void SettingsTest::test_numbers() {
    Settings settings;

    CPPUNIT_ASSERT(settings.matrix_to_state(0, true) == 2);
    CPPUNIT_ASSERT(settings.matrix_to_state(1, true) == 4);
    CPPUNIT_ASSERT(settings.matrix_to_state(0, false) == 1);
    CPPUNIT_ASSERT(settings.matrix_to_state(1, false) == 3);

    CPPUNIT_ASSERT(settings.state_to_matrix(2) == 0);
    CPPUNIT_ASSERT(settings.state_to_matrix(4) == 1);
    CPPUNIT_ASSERT(settings.state_to_matrix(1) == 0);
    CPPUNIT_ASSERT(settings.state_to_matrix(3) == 1);
}
