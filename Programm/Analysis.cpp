// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Analysis.hpp"

#include "BootstrapSample.hpp"
#include "GEVPSolver.hpp"

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
} {

    for (unsigned t : settings.correlation_ts) {
        std::map<unsigned, BootstrappedQuantity> inner;
        for (unsigned n {0}; n < settings.max_energyvalue(); n++) {
            inner.emplace(std::piecewise_construct, std::make_tuple(n), std::make_tuple());
        }

        bs_E_n_t.emplace(t, std::move(inner));
    }

    if (settings.bootstrap_samples > 0) {
        create_samples();
        save_eigenvalues();
        save_c11();
        save_c22();
        save_histogram();
    }
}

void Analysis::insert_eigenvalues(CorrFunc &C, bool even, BQMapMap &bs_lambda_n_t) {
    for (unsigned t : settings.correlation_ts) {
        unsigned t_0 = settings.get_t_0(t);
        if (t <= t_0) {
            continue;
        }
        auto &C_t0 = C[t_0];
        std::vector<double> lambda_i_t (GEVPSolver::eigenvalues(C[t], C_t0));

        // Remove the first even eigenvalue here since that does not give an
        // energy eigenvalue.
        unsigned cutoff {even ? 1u : 0u};
        for (unsigned i {cutoff}; i < lambda_i_t.size(); i++) {
            double lambda {lambda_i_t[i]};
            bs_lambda_n_t[t][settings.matrix_to_state(i - cutoff, even)].append(lambda);
        }
    }
}

void Analysis::create_samples() {
    unsigned largest = *std::max_element(settings.correlation_ts.begin(), settings.correlation_ts.end());
    c11 = std::vector<BootstrappedQuantity>(largest + 1);
    c22 = std::vector<BootstrappedQuantity>(largest + 1);

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
        BootstrapSample sample {pool, settings};

        // Extract C_11.
        CorrFunc &odd {sample.odd};
        for (std::pair<const unsigned, Eigen::MatrixXd> &pair : odd) {
            unsigned t {pair.first};
            Eigen::MatrixXd &c_t {pair.second};
            double c_t_11 {c_t(settings.state_to_matrix(1), settings.state_to_matrix(1))};
            c11[t].append(c_t_11);
        }

        // Extract C_22.
        CorrFunc &even {sample.even};
        for (std::pair<const unsigned, Eigen::MatrixXd> &pair : even) {
            unsigned t {pair.first};
            Eigen::MatrixXd &c_t {pair.second};
            double c_t_22 {c_t(settings.state_to_matrix(1), settings.state_to_matrix(1))};
            c22[t].append(c_t_22);
        }

        // Extract histogram.
        boot_hist.insert_histogram(sample.histogram);

        insert_eigenvalues(sample.even, true, bs_E_n_t);
        insert_eigenvalues(sample.odd, false, bs_E_n_t);

        bar.update(sample_id);
    }
}

void Analysis::save_eigenvalues() {
    // Output to single files.
    for (unsigned n {1}; n <= settings.max_energyvalue(); n++) {
        std::ostringstream filename;
        std::ostringstream output;
        filename << "eigenvalue-" << std::setfill('0') << std::setw(2) << n << ".csv";
        for (unsigned t : settings.correlation_ts) {
            try {
                double mean {bs_E_n_t[t][n].mean()};
                double stddev {bs_E_n_t[t][n].stddev()};
                output << t *settings.time_step << "\t" << mean << "\t" << stddev << std::endl;
            }
            catch (std::runtime_error e) {
            }
        }
        std::string output_string {output.str()};
        if (output_string.length() > 0) {
            std::ofstream handle {settings.generate_filename(filename.str())};
            handle << settings.report();
            handle << "# tau \t c??_val \t c??_err" << std::endl;
            handle << output_string;
        }
    }
}

void Analysis::save_c11() {
    std::ofstream c11_handle {settings.generate_filename("c11.csv")};
    c11_handle << settings.report();
    c11_handle << "# tau \t c11_val \t c11_err" << std::endl;
    //for (unsigned i {0}; i < c11.size(); i++) {
    for (unsigned i : settings.correlation_ts) {
        try {
            c11_handle << i *settings.time_step << "\t" << c11[i].mean() << "\t" << c11[i].stddev() << std::endl;
        }
        catch (std::runtime_error e) {
            std::cout << "Element " << i << ": " << e.what() << std::endl;
        }
    }
}

// XXX
void Analysis::save_c22() {
    std::ofstream c22_handle {settings.generate_filename("c22.csv")};
    c22_handle << settings.report();
    c22_handle << "# tau \t c22_val \t c22_err" << std::endl;
    //for (unsigned i {0}; i < c22.size(); i++) {
    for (unsigned i : settings.correlation_ts) {
        try {
            c22_handle << i *settings.time_step << "\t" << c22[i].mean() << "\t" << c22[i].stddev() << std::endl;
        }
        catch (std::runtime_error e) {
            std::cout << "Element " << i << ": " << e.what() << std::endl;
        }
    }
}

void Analysis::save_histogram() {
    boot_hist.write_histogram(settings.generate_filename("position-density.csv"));
}
