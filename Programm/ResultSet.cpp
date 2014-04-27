// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "ResultSet.hpp"
#include "Settings.hpp"

#include <iostream>
#include <random>
#include <thread>

typedef std::pair<const unsigned int, std::shared_ptr<Correlation>> correlation_pair;

ResultSet::ResultSet(BootstrapPool &pool, Settings &settings)
    :
    dens {PositionDensity{ -5, 5, settings.position_hist_bins}},
pool(pool) {
    computables.emplace_back(new Moment {1});
    computables.emplace_back(new Moment {2});

    add_correlation(0);
    add_correlation(1);
    add_correlation(2);
    add_correlation(3);
    add_correlation(4);
    add_correlation(5);

    compute_using_pool();
}

void ResultSet::add_correlation(unsigned int distance) {
    std::shared_ptr<Correlation> next_correlation { new Correlation {distance}};
    correlations.insert(correlation_pair {
        distance,
        std::shared_ptr<Correlation>{next_correlation}
    });

    computables.push_back(next_correlation);
}

void ResultSet::operator()() {
    int sample_id;
    // TODO Put this into the Settings.
    while ((sample_id = counter()) < 1000) {
        if (sample_id % 20 == 0) {
            std::cout << "Creating BoostrapSample " << sample_id << std::endl;
        }
        BootstrapSample sample {pool, engine};

        for (auto computable : computables) {
            computable->add_sample(sample);
        }

        dens.add_sample(sample);
    }
}

void ResultSet::compute_using_pool() {
    unsigned int cpu_count = std::thread::hardware_concurrency();
    //cpu_count = 1;

    std::vector<std::thread> workers;
    for (unsigned int i = 0; i < cpu_count; i++) {
        workers.emplace_back(std::ref(*this));
    }

    for (auto & worker : workers) {
        worker.join();
    }
}

void ResultSet::print_results() {
    for (auto computable : computables) {
        std::cout << computable->get_name() << ": " << computable->format() << std::endl;
    }

    std::ofstream of {"out/histogram-position-resultset.csv"};
    dens.write_histogram(of);
    of.close();
}
