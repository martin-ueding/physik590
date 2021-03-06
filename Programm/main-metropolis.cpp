// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Analysis.hpp"
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

    settings.estimate_memory_usage();

    ProgressBar driver_bar{"Creating thermalized state",
                           settings.pre_iterations};
    MetropolisDriver m_driver{settings};
    driver_bar.close();

    std::shared_ptr<BootstrapPool> pool;

    if (settings.load_filename == "") {
        std::cout << "ID of this run: " << settings.hash() << std::endl;
        pool = std::unique_ptr<BootstrapPool>{
            new BootstrapPool{m_driver, settings}};
        save_pool(pool, settings);
    } else {
        load_into_pool(pool, settings);
        std::cout << "ID of this analysis: " << settings.hash() << std::endl;
    }

    settings.store_json(settings.generate_filename("options.js"));

    Analysis analysis(*pool, settings);

    return 0;
}
