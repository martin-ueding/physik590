// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "ResultSet.hpp"

#include <iostream>
#include <random>
#include <thread>

ResultSet::ResultSet(BootstrapPool &pool) : pool(pool) {
    computables.emplace_back(new Moment {1});
    computables.emplace_back(new Moment {2});
    computables.emplace_back(new Correlation {0});
    computables.emplace_back(new Correlation {1});
    computables.emplace_back(new Correlation {2});
    computables.emplace_back(new Correlation {3});
    computables.emplace_back(new Correlation {4});
    computables.emplace_back(new Correlation {5});
    computables.emplace_back(new Correlation {6});
    computables.emplace_back(new Correlation {7});
    computables.emplace_back(new Correlation {8});
    computables.emplace_back(new Correlation {9});
    computables.emplace_back(new Correlation {10});
    computables.emplace_back(new Correlation {11});
    computables.emplace_back(new Correlation {12});
    computables.emplace_back(new Correlation {13});
    computables.emplace_back(new Correlation {14});
    computables.emplace_back(new Correlation {15});
    computables.emplace_back(new Correlation {16});
    computables.emplace_back(new Correlation {20});
    computables.emplace_back(new Correlation {40});
    computables.emplace_back(new Correlation {80});
    computables.emplace_back(new Correlation {160});
    computables.emplace_back(new Correlation {320});
    computables.emplace_back(new Correlation {640});
    computables.emplace_back(new Correlation {998});

    compute_using_pool();
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

    for (auto &worker : workers) {
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
