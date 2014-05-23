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

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>

#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>

typedef std::map<unsigned, std::map<unsigned, BootstrappedQuantity>> BQMapMap;

void insert_eigenvalues(CorrFunc &C, bool even, BQMapMap &bs_E_n_t, Settings &settings) {
    auto &C_t0 = C[settings.t_0];
    for (unsigned t : settings.correlation_ts) {
        if (t <= settings.t_0) {
            continue;
        }
        std::vector<double> lambda_i_t (GEVPSolver::eigenvalues(C[t], C_t0));
        //std::vector<double> lambda_n_tplus1 {GEVPSolver::eigenvalues(sample.even[t+1], C_t0)};

        for (unsigned i {0}; i < lambda_i_t.size(); i++) {
            //double E = - 1 / settings.time_step * std::log(lambda_n_tplus1[n] / lambda_n_t[n]);
            double E = lambda_i_t[i];
            bs_E_n_t[t][settings.matrix_to_state(i, even)].append(E);
        }
    }
}

void save_pool(std::shared_ptr<BootstrapPool> pool, Settings &settings) {
    ProgressBar bar {"Serializing", 1};
    std::ofstream ofs {settings.generate_filename("pool.bin")};
    boost::archive::binary_oarchive oa {ofs};
    oa << *pool;
    //oa << settings;
}

void load_into_pool(std::shared_ptr<BootstrapPool> &pool, Settings &settings) {
    ProgressBar bar {"Loading data", 1};
    pool = std::unique_ptr<BootstrapPool> {new BootstrapPool {}};
    std::ifstream ifs(settings.load_filename);
    boost::archive::binary_iarchive ia(ifs);
    ia >> *pool;
    //ia >> settings;

    bar.close();
    std::cout << "pool.even.size() " << pool->even.size() << std::endl;
    std::cout << "pool.odd.size() " << pool->odd.size() << std::endl;
    std::cout << "pool.histograms.size() " << pool->histograms.size() << std::endl;
    std::cout << "pool.histograms[0].size() " << pool->histograms[0].size() << std::endl;
    std::cout << "pool.histograms[0].get_min() " << pool->histograms[0].get_min() << std::endl;
    std::cout << "pool.histograms[0].get_max() " << pool->histograms[0].get_max() << std::endl;
    std::cout << "pool.histograms[0][0] " << pool->histograms[0][0] << std::endl;
}

void analysis(BootstrapPool &pool, Settings &settings) {
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


        insert_eigenvalues(sample.even, true, bs_E_n_t, settings);
        insert_eigenvalues(sample.odd, false, bs_E_n_t, settings);


        sample_bar.update(sample_id);
    }
    sample_bar.close();


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

    std::ofstream c11_handle {settings.generate_filename("c11.csv")};
    c11_handle << settings.report();
    c11_handle << "# tau \t c11_val \t c11_err" << std::endl;
    for (unsigned i {0}; i < c11.size(); i++) {
        c11_handle << i *settings.time_step << "\t" << c11[i].mean() << "\t" << c11[i].stddev() << std::endl;
    }


    boot_hist.write_histogram(settings.generate_filename("position-density.csv"));
}

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

    analysis(*pool, settings);

    return 0;
}
