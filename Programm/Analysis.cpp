// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Analysis.hpp"

#include "BootstrapSample.hpp"
#include "GEVPSolver.hpp"
#include "LinearFit.hpp"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <thread>

Analysis::Analysis(BootstrapPool &pool, Settings &settings) :
    pool {pool},
     settings {settings},
boot_hist {BootstrappedHistogram{
        settings.position_hist_min, settings.position_hist_max,
        settings.position_hist_bins
    }
},
    energies{std::vector<BootstrappedQuantity>(settings.max_energyvalue())}
{

    t.resize(settings.correlation_ts.size());
    for (size_t i{0}; i != settings.correlation_ts.size(); ++i) {
        t[i] = settings.time_step * settings.correlation_ts.size();
    }

    create_samples();
    save_c11();
    save_histogram();

    for (size_t i{0}; i != energies.size(); ++i) {
        std::cout << "E_" << i+1 << "\t" << energies[i].mean() << "\t" << energies[i].stddev() << std::endl;
    }
}

void Analysis::insert_eigenvalues(CorrFunc &C, bool even) {
    std::map<unsigned, std::vector<double>> lambda_n_t;
    for (unsigned t : settings.correlation_ts) {
        unsigned t_0 = settings.get_t_0(t);
        if (t <= t_0) {
            continue;
        }
        auto &C_t0 = C[settings.t_0];
        std::vector<double> lambda_i_t (GEVPSolver::eigenvalues(C[t], C_t0));

        for (unsigned i {0}; i < lambda_i_t.size(); i++) {
            double lambda {lambda_i_t[i]};
            std::vector<double> &lambda_t {lambda_n_t[settings.matrix_to_state(i, even)]};
            lambda_t.push_back(lambda);
        }
    }

    for (auto pair : lambda_n_t) {
        unsigned n {pair.first};
        std::vector<double> &lambda_t {pair.second};
        std::vector<double> lambda_t_transformed (lambda_t.size());
        std::vector<double> lambda_t_err_transformed (lambda_t.size());
        for (size_t i{0}; i != lambda_t.size(); ++i) {
            lambda_t_transformed[i] = std::log(lambda_t[i]);
            lambda_t_err_transformed[i] = 1 / lambda_t[i];
        }

        LinearFit fit {t, lambda_t_transformed, lambda_t_err_transformed};
        energies[n-1].append(fit.c1);
    }
}

void Analysis::create_samples() {
    unsigned largest = *std::max_element(settings.correlation_ts.begin(), settings.correlation_ts.end());
    c11 = std::vector<BootstrappedQuantity>(largest + 1);

    ProgressBar bar {"Creating bootstrap samples", settings.bootstrap_samples};

    std::vector<std::thread> workers;
    for (unsigned i {0}; i < settings.max_cores; i++) {
        workers.emplace_back([&]() {
            worker(bar);
        });
    }
    for (unsigned i {0}; i < settings.max_cores; i++) {
        workers[i].join();
    }
}

void Analysis::worker(ProgressBar &bar) {
    unsigned sample_id;
    while ((sample_id = sample_id_atom++) < settings.bootstrap_samples) {
        BootstrapSample sample {pool};

        // Extract C_11.
        CorrFunc &odd {sample.odd};
        for (std::pair<const unsigned, Eigen::MatrixXd> &pair : odd) {
            unsigned t {pair.first};
            Eigen::MatrixXd &c_t {pair.second};
            double c_t_11 {c_t(settings.state_to_matrix(1), settings.state_to_matrix(1))};
            c11[t].append(c_t_11);

        }

#ifndef NDEBUG
        if (sample_id == 1) {
            std::cout << std::endl;
            std::cout << "even[39]:" << std::endl;
            std::cout << sample.even[39] << std::endl;
            std::cout << std::endl;
            std::cout << "odd[39]:" << std::endl;
            std::cout << sample.odd[39] << std::endl;
            std::cout << std::endl;
        }
#endif


        // Extract histogram.
        boot_hist.insert_histogram(sample.histogram);

        insert_eigenvalues(sample.even, true);
        insert_eigenvalues(sample.odd, false);

        bar.update(sample_id);
    }
}

void Analysis::save_c11() {
    std::ofstream c11_handle {settings.generate_filename("c11.csv")};
    c11_handle << settings.report();
    c11_handle << "# tau \t c11_val \t c11_err" << std::endl;
    for (unsigned i {0}; i < c11.size(); i++) {
        try {
            c11_handle << i *settings.time_step << "\t" << c11[i].mean() << "\t" << c11[i].stddev() << std::endl;
        }
        catch (std::runtime_error e) {
            std::cout << "Element " << i << ": " << e.what() << std::endl;
        }
    }
}

void Analysis::save_histogram() {
    boot_hist.write_histogram(settings.generate_filename("position-density.csv"));
}
