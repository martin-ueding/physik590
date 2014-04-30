// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "HarmonicOscillator.hpp"

class HarmonicOscillatorTest : public CppUnit::TestFixture {
        CPPUNIT_TEST_SUITE(HarmonicOscillatorTest);
        CPPUNIT_TEST(test_sign);
        CPPUNIT_TEST_SUITE_END();

    public:
        void test_sign();
};
