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

        ///////////////////////////////////////////////////////////////////////
        sample_bar.close();

        std::map<unsigned, std::vector<double>> lambda_n_t;

        unsigned t_0 = 0;
        auto &C_t0 = sample.even[t_0];
        for (auto &pair : sample.even) {
            unsigned t = pair.first;
            auto &C_t = pair.second;
            lambda_n_t.insert(decltype(lambda_n_t)::value_type{t, GEVPSolver::eigenvalues(C_t, C_t0)});

            for (double lambda : lambda_n_t[t]) {
                std::cout << lambda << std::endl;
            }
        }

        return 0;
        ///////////////////////////////////////////////////////////////////////

        sample_bar.update(sample_id);
    }
    sample_bar.close();

    boot_hist.write_histogram(settings.generate_filename("position-density.csv"));

    return 0;
}

// Extract the correlation matrix function from the sample.
// Compute the λ_n(t).
// Calculate the E_n(t).
