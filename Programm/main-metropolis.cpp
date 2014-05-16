// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapPool.hpp"
#include "GEVPSolver.hpp"
#include "MetropolisDriver.hpp"
#include "parse_arguments.hpp"
#include "ProgressBar.hpp"
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

    BootstrappedHistogram boot_hist {-5, 5, settings.position_hist_bins};

    ProgressBar sample_bar {"Creating bootstrap samples", settings.bootstrap_samples};
    for (unsigned sample_id {0u}; sample_id < settings.bootstrap_samples; sample_id++) {
        BootstrapSample sample {pool};

        boot_hist.insert_histogram(sample.histogram);

        std::vector<double> eigenvalues = GEVPSolver::eigenvalues(sample.even[2], sample.even[1]);
        sample_bar.close();

        for (auto i : eigenvalues) {
            std::cout << i << std::endl;
        }

        return 0;

        sample_bar.update(sample_id);
    }
    sample_bar.close();

    boot_hist.write_histogram(settings.generate_filename("position-density.csv"));

    return 0;
}
