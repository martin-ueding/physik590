// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "HarmonicOscillatorTest.hpp"

void HarmonicOscillatorTest::test_sign() {
    HarmonicOscillator ho{0.1, 1.0, 1.0};

    // Check that the action increases when the trajectory gets a kink.
    CPPUNIT_ASSERT(ho.action_difference(0.0, 0.0, 1.0, 0.0) > 0);

    // Check that the action decreases when the trajectory is flattened.
    CPPUNIT_ASSERT(ho.action_difference(0.0, 1.0, 0.0, 0.0) > 0);

    // Check that the action does not change when the trajectory does not
    // change either.
    CPPUNIT_ASSERT(ho.action_difference(1.0, 1.0, 1.0, 1.0) == 0);
}
