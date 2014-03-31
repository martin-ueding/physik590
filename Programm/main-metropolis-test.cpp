#include <cppunit/TextTestRunner.h>

#include "test-periodic.hpp"

int main() {
    CppUnit::TextTestRunner runner;
    runner.addTest(PeriodicTest::suite());
    runner.run();
    return 0;
}

