// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "analysis.hpp"
#include "BootstrappedQuantity.hpp"
#include "BootstrapPool.hpp"
#include "GEVPSolver.hpp"
#include "MetropolisDriver.hpp"
#include "parse_arguments.hpp"
#include "ProgressBar.hpp"
#include "ResultSet.hpp"
#include "Settings.hpp"

#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>

/**
  Entry point for the metropolis program.

  @param argc Argument count
  @param argv Argument values
  @return Return code
  */
int main(int argc, char **argv) {
    Settings settings;

    parse_arguments(argc, argv, settings);
    settings.compute();

    std::cout << "ID of this run: " << settings.hash() << std::endl;

    MetropolisDriver m_driver {settings};

    std::shared_ptr<BootstrapPool> pool;

    if (settings.load_filename == "") {
        pool = std::unique_ptr<BootstrapPool> {new BootstrapPool {m_driver, settings}};
        save_pool(pool, settings);
    }
    else {
        load_into_pool(pool, settings);
    }

    std::cout << "ID of this run: " << settings.hash() << std::endl;

    analysis::analysis(*pool, settings);

    return 0;
}
