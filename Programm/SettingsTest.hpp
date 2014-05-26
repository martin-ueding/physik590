// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <cppunit/extensions/HelperMacros.h>

class SettingsTest : public CppUnit::TestFixture {
        CPPUNIT_TEST_SUITE(SettingsTest);
        CPPUNIT_TEST(test_numbers);
        CPPUNIT_TEST_SUITE_END();

    public:
        void test_numbers();
};
