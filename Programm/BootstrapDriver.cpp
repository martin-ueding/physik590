// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapDriver.hpp"

#include "MetropolisDriver.hpp"

void BootstrapDriver::run(Settings &s, int iterations) {
    MetropolisDriver m_driver {s};

    for (int i {0}; i < iterations; i++) {
        m_driver.run(results);
    }
}

void BootstrapDriver::print_results() {
    results.first_moment.print();
    results.second_moment.print();
    results.sigma.print();
    results.energy0.print();
    results.accept_rate.print();
}
