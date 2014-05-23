// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Analysis.hpp"

#include "BootstrapSample.hpp"
#include "GEVPSolver.hpp"

#include <fstream>

Analysis::Analysis(BootstrapPool &pool, Settings &settings) :
    pool {pool},
settings {settings},
     boot_hist {BootstrappedHistogram{
        settings.position_hist_min, settings.position_hist_max,
        settings.position_hist_bins
    }} {

    for (unsigned t : settings.correlation_ts) {
        std::map<unsigned, BootstrappedQuantity> inner;
        for (unsigned n {0}; n < settings.max_energyvalue(); n++) {
            inner.emplace(std::piecewise_construct, std::make_tuple(n), std::make_tuple());
        }

        bs_E_n_t.emplace(t, std::move(inner));
    }

    create_samples();
    save_eigenvalues();
    save_c11();
    save_histogram();
}

void Analysis::insert_eigenvalues(CorrFunc &C, bool even, BQMapMap &bs_lambda_n_t) {
    auto &C_t0 = C[settings.t_0];
    for (unsigned t : settings.correlation_ts) {
        if (t <= settings.t_0) {
            continue;
        }
        std::vector<double> lambda_i_t (GEVPSolver::eigenvalues(C[t], C_t0));

        for (unsigned i {0}; i < lambda_i_t.size(); i++) {
            double lambda = lambda_i_t[i];
            bs_lambda_n_t[t][settings.matrix_to_state(i, even)].append(lambda);
        }
    }
}

void Analysis::create_samples() {
    unsigned largest = *std::max_element(settings.correlation_ts.begin(), settings.correlation_ts.end());
    c11 = std::vector<BootstrappedQuantity>(largest + 2);

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


        insert_eigenvalues(sample.even, true, bs_E_n_t);
        insert_eigenvalues(sample.odd, false, bs_E_n_t);


        sample_bar.update(sample_id);
    }
}

void Analysis::save_eigenvalues() {
    // Output to single files.
    for (unsigned n {1}; n <= settings.max_energyvalue(); n++) {
        std::ostringstream filename;
        std::ostringstream output;
        filename << "eigenvalue-" << n << ".csv";
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
    for (unsigned i {0}; i < c11.size(); i++) {
        c11_handle << i *settings.time_step << "\t" << c11[i].mean() << "\t" << c11[i].stddev() << std::endl;
    }
}

void Analysis::save_histogram() {
    boot_hist.write_histogram(settings.generate_filename("position-density.csv"));
}
