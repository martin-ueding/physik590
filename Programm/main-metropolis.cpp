// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrappedQuantity.hpp"
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
    settings.compute();

    for (auto i : settings.correlation_ts) {
        std::cout << i << std::endl;
    }

    std::cout << "ID of this run: " << settings.hash() << std::endl;

    MetropolisDriver m_driver {settings};

    BootstrapPool pool {m_driver, settings};

    BootstrappedHistogram boot_hist {
        settings.position_hist_min, settings.position_hist_max,
            settings.position_hist_bins
    };

    /**
      The inner map is a mapping that goes through all E_n. The outter map
      holds the E_n at different times t.
      */
    std::map<unsigned, std::map<unsigned, BootstrappedQuantity>> bs_E_n_t;

    for (unsigned t : settings.correlation_ts) {
        std::map<unsigned, BootstrappedQuantity> inner;
        for (unsigned n {0}; n < settings.max_energyvalue(); n++) {
            inner.emplace(std::piecewise_construct, std::make_tuple(n), std::make_tuple());
        }

        bs_E_n_t.emplace(t, std::move(inner));
    }

    ProgressBar sample_bar {"Creating bootstrap samples", settings.bootstrap_samples};
    for (unsigned sample_id {0u}; sample_id < settings.bootstrap_samples; sample_id++) {
        BootstrapSample sample {pool};

        boot_hist.insert_histogram(sample.histogram);

        unsigned t_0 = 0;
        auto &C_t0 = sample.even[t_0];
        for (unsigned t : settings.correlation_ts) {
            std::vector<double> lambda_n_t {GEVPSolver::eigenvalues(sample.even[t], C_t0)};
            std::vector<double> lambda_n_tplus1 {GEVPSolver::eigenvalues(sample.even[t+1], C_t0)};

            for (unsigned n {0}; n < lambda_n_t.size(); n++) {
                double E = - 1 / settings.time_step * std::log(lambda_n_tplus1[n] / lambda_n_t[n]);

                bs_E_n_t[t][settings.energyvalue(n, true)].append(E);
            }
        }

        sample_bar.update(sample_id);
    }
    sample_bar.close();


    for (unsigned n {0}; n < settings.max_energyvalue(); n++) {
        for (unsigned t : settings.correlation_ts) {
            auto ms = bs_E_n_t[t][n].mean_and_stddev();
            std::cout << "E_" << n << "(" << t << ") = " << ms.first << "±" << ms.second << std::endl;
        }
    }

    boot_hist.write_histogram(settings.generate_filename("position-density.csv"));

    return 0;
}

// Extract the correlation matrix function from the sample.
// Compute the λ_n(t).
// Calculate the E_n(t).
