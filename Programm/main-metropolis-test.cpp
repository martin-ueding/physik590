// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include <cppunit/TextTestRunner.h>

#include "SettingsTest.hpp"

int main() {
    CppUnit::TextTestRunner runner;
    runner.addTest(SettingsTest::suite());
    runner.run();
    return 0;
}
