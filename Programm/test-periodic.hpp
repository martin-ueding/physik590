#include <cppunit/extensions/HelperMacros.h>

class PeriodicTest: public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(PeriodicTest);
    CPPUNIT_TEST(test_periodic);
    CPPUNIT_TEST_SUITE_END();

    public:
    void test_periodic();
};
