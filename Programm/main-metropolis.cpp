// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapPool.hpp"
#include "MetropolisDriver.hpp"
#include "parse_arguments.hpp"
#include "ResultSet.hpp"
#include "Settings.hpp"

#include <iostream>

/**
  Entry point for the metropolis program.

  @param argc Argument count
  @param argv Argument values
  @return Return code
  */
int main(int argc, char **argv) {
    Settings settings;

    if (parse_arguments(argc, argv, settings)) {
        return 0;
    }

    std::cout << "ID of this run: " << settings.hash() << std::endl;

    MetropolisDriver m_driver {settings};

    BootstrapPool pool {m_driver, settings.iterations, settings.position_hist_bins};

    return 0;
}
