// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapPool.hpp"

#include "Correlation.hpp"

#include <fstream>
#include <thread>

BootstrapPool::BootstrapPool() {
}

BootstrapPool::BootstrapPool(MetropolisDriver &driver, Settings &settings) {
    ProgressBar bar{"Populating bootstrap pool", settings.iterations};

    std::vector<std::thread> workers;

    even.resize(settings.iterations);
    odd.resize(settings.iterations);
    e0_virial.resize(settings.iterations);
    histograms.resize(settings.iterations);

    for (unsigned i{0}; i < settings.max_cores; i++) {
        workers.emplace_back([this, settings, &bar, driver, i]() {
            worker(settings, bar, driver, i);
        });
    }
    for (unsigned i{0}; i < settings.max_cores; i++) {
        workers[i].join();
    }
    bar.close();

    std::cout << accept_rate_output.str();
}

void BootstrapPool::worker(Settings settings,
                           ProgressBar &bar_corr,
                           MetropolisDriver driver,
                           int seed) {
    unsigned t_id;
    driver.ma.re_seed(seed + 10);
    while ((t_id = t_id_atom++) < settings.iterations) {
        // Reserve space for correlation lists.
        CorrList list_even;
        CorrList list_odd;
        list_even.reserve(settings.correlation_ts.size());
        list_odd.reserve(settings.correlation_ts.size());

        // Retrieve the next trajectory.
        std::vector<double> trajectory = driver.next();

        // Compute the histogram.
        Histogram h{settings.position_hist_min,
                    settings.position_hist_max,
                    settings.position_hist_bins};
        for (auto x_j : trajectory) {
            h(x_j);
        }

        // Compute the powers of the trajectory so that the power function does
        // not need to be invoked that often. I have not tested, but I assume
        // that this is a hotspot.
        std::vector<std::vector<double>> powers_even(settings.correlation_size);
        std::vector<std::vector<double>> powers_odd(settings.correlation_size);

        for (unsigned row{0u}; row < settings.correlation_size; row++) {
            unsigned power_even{settings.matrix_to_state(row, true)};
            unsigned power_odd{settings.matrix_to_state(row, false)};
            assert(power_even > 0);
            assert(power_odd > 0);

            // Bring the memory to the required size.
            powers_even[row].resize(settings.time_sites);
            powers_odd[row].resize(settings.time_sites);

            // Take the power from all the x[k].
            for (unsigned k{0u}; k < settings.time_sites; ++k) {
                powers_even[row][k] = std::pow(trajectory[k], power_even);
                powers_odd[row][k] = std::pow(trajectory[k], power_odd);
            }
        }

        // Compute the correlations.
        for (unsigned distance : settings.correlation_ts) {
            list_even.push_back(
                correlation(trajectory, powers_even, settings, distance, true));
            list_odd.push_back(
                correlation(trajectory, powers_odd, settings, distance, false));
        }

        // Compute the ground state energy with the virial theorem.
        double gauss_width_squared =
            settings.gauss_width * settings.gauss_width;
        double pi{std::atan(1) * 4};
        double prefactor = 1. / std::sqrt(2. * pi * gauss_width_squared) * 2. *
                           settings.inverse_scattering_length;

        std::vector<double> &squares = powers_even[0];
        double mean_squared{0};
        double mean_gauss{0};
        double mean_gauss_x2{0};
        for (double square : squares) {
            mean_squared += square;
            mean_gauss += std::exp(-square / gauss_width_squared);
            mean_gauss_x2 += std::exp(-square / gauss_width_squared) * square;
        }
        mean_squared /= squares.size();
        mean_gauss /= squares.size();
        mean_gauss_x2 /= squares.size();

        double e0 =
            settings.mu_squared * mean_squared +
            prefactor * (1 / gauss_width_squared * mean_gauss_x2 - mean_gauss);

        // Insert data into mutual data structures.
        std::lock_guard<std::mutex>{mutex};
        histograms[t_id] = h;
        even[t_id] = list_even;
        odd[t_id] = list_odd;
        e0_virial[t_id] = e0;

        bar_corr.update(t_id);
    }

    std::lock_guard<std::mutex>{accept_rate_mutex};
    accept_rate_output << "Accept rate:             "
                       << driver.ma.get_accept_rate() << std::endl;
    accept_rate_output << "Accept rate negative:    "
                       << driver.ma.get_accept_rate_negative() << std::endl;
    accept_rate_output << "Accept rate exponential: "
                       << driver.ma.get_accept_rate_exponential() << std::endl;
}

void save_pool(std::shared_ptr<BootstrapPool> pool, Settings &settings) {
    ProgressBar bar{"Serializing", 1};
    std::ofstream ofs{settings.generate_filename("pool.bin")};
    boost::archive::binary_oarchive oa{ofs};
    try {
        oa << *pool;
        oa << settings;
    } catch (boost::archive::archive_exception ex) {
        std::cout << ex.what() << std::endl;
    }
}

void load_into_pool(std::shared_ptr<BootstrapPool> &pool, Settings &settings) {
    ProgressBar bar{"Loading data", 1};
    pool = std::unique_ptr<BootstrapPool>{new BootstrapPool{}};
    std::ifstream ifs(settings.load_filename);
    boost::archive::binary_iarchive ia(ifs);
    ia >> *pool;
    ia >> settings;

#ifndef NDEBUG
    bar.close();

    std::cout << "pool.even.size() " << pool->even.size() << std::endl;
    std::cout << "pool.odd.size() " << pool->odd.size() << std::endl;
    std::cout << "pool.histograms.size() " << pool->histograms.size()
              << std::endl;
    std::cout << "pool.histograms[0].size() " << pool->histograms[0].size()
              << std::endl;
    std::cout << "pool.histograms[0].get_min() "
              << pool->histograms[0].get_min() << std::endl;
    std::cout << "pool.histograms[0].get_max() "
              << pool->histograms[0].get_max() << std::endl;
    std::cout << "pool.histograms[0][0] " << pool->histograms[0][0]
              << std::endl;

    std::cout << std::endl;
    std::cout << settings.report() << std::endl;
#endif
}
