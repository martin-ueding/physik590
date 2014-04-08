// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapDriver.hpp"

#include "MetropolisDriver.hpp"

void BootstrapDriver::run(Settings &s) {
    MetropolisDriver m_driver {s};

    m_driver.run(results);
}
