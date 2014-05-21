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

#include <fstream>
#include <iostream>
#include <iomanip>

typedef std::map<unsigned, std::map<unsigned, BootstrappedQuantity>> BQMapMap;

/**
  This method does some stuff, and I have yet to specify what exactly it does.
  */
void do_stuff(CorrFunc &C, bool even, BQMapMap &bs_E_n_t, Settings &settings) {
    unsigned t_0 = 0;
    auto &C_t0 = C[t_0];
    for (unsigned t : settings.correlation_ts) {
        std::vector<double> lambda_i_t ( GEVPSolver::eigenvalues(C[t], C_t0) );
        //std::vector<double> lambda_n_tplus1 {GEVPSolver::eigenvalues(sample.even[t+1], C_t0)};

        for (unsigned i {0}; i < lambda_i_t.size(); i++) {
            //double E = - 1 / settings.time_step * std::log(lambda_n_tplus1[n] / lambda_n_t[n]);
            double E = lambda_i_t[i];
            bs_E_n_t[t][settings.matrix_to_state(i, even)].append(E);
        }
    }
}

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
    BQMapMap bs_E_n_t;

    for (unsigned t : settings.correlation_ts) {
        std::map<unsigned, BootstrappedQuantity> inner;
        for (unsigned n {0}; n < settings.max_energyvalue(); n++) {
            inner.emplace(std::piecewise_construct, std::make_tuple(n), std::make_tuple());
        }

        bs_E_n_t.emplace(t, std::move(inner));
    }

    unsigned largest = *std::max_element(settings.correlation_ts.begin(), settings.correlation_ts.end());
    std::vector<BootstrappedQuantity> c11(largest + 2);

    ProgressBar sample_bar {"Creating bootstrap samples", settings.bootstrap_samples};
    for (unsigned sample_id {0u}; sample_id < settings.bootstrap_samples; sample_id++) {
        BootstrapSample sample {pool};

        // Extract C_11.
        CorrFunc &odd {sample.odd};
        for (std::pair<const unsigned, Eigen::MatrixXd> &pair : odd) {
            unsigned t {pair.first};
            Eigen::MatrixXd &c_t {pair.second};
            double c_t_11 {c_t(settings.state_to_matrix(1), settings.state_to_matrix(1))};
            c11[t].append(c_t_11);
        }


        // Extract histogram.
        boot_hist.insert_histogram(sample.histogram);


        do_stuff(sample.even, true, bs_E_n_t, settings);
        do_stuff(sample.odd, false, bs_E_n_t, settings);


        sample_bar.update(sample_id);
    }
    sample_bar.close();


    for (unsigned n {1}; n <= settings.max_energyvalue(); n++) {
        for (unsigned t : settings.correlation_ts) {
            std::cout << "E_" << n << "(" << std::setw(4) << t << ") = ";
            try {
                double mean {bs_E_n_t[t][n].mean()};
                double stddev {bs_E_n_t[t][n].stddev()};
                std::cout << std::setw(13) << mean << " ± " << std::setw(13) << stddev << std::endl;
            }
            catch (std::runtime_error e) {
                std::cout << e.what() << std::endl;
            }
        }
    }

    // Output to single files.
    for (unsigned n {1}; n <= settings.max_energyvalue(); n++) {
        std::ostringstream filename;
        std::ostringstream output;
        filename << "eigenvalue-" << n << ".csv";
        for (unsigned t : settings.correlation_ts) {
            try {
                double mean {bs_E_n_t[t][n].mean()};
                double stddev {bs_E_n_t[t][n].stddev()};
                output << t << "\t" << mean << "\t" << stddev << std::endl;
            }
            catch (std::runtime_error e) {
            }
        }
        std::string output_string {output.str()};
        if (output_string.length() > 0) {
            std::ofstream handle {settings.generate_filename(filename.str())};
            handle << settings.report();
            handle << output_string;
        }
    }

    std::ofstream c11_handle {settings.generate_filename("c11.csv")};
    c11_handle << settings.report();
    c11_handle << "tau \t c11_val \t c11_err" << std::endl;
    for (unsigned i{0}; i < c11.size(); i++) {
        c11_handle << i * settings.time_step << "\t" << c11[i].mean() << "\t" << c11[i].stddev() << std::endl;
    }


    boot_hist.write_histogram(settings.generate_filename("position-density.csv"));

    return 0;
}

// Extract the correlation matrix function from the sample.
// Compute the λ_n(t).
// Calculate the E_n(t).
