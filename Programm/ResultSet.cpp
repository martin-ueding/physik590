// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "ResultSet.hpp"

#include <iostream>
#include <random>

ResultSet::ResultSet(BootstrapPool pool) {
    computables.push_back(&mean);
    computables.push_back(&moment_2);

    compute_using_pool(pool);
}

void ResultSet::compute_using_pool(BootstrapPool pool) {
    std::mt19937 engine;

    for (size_t sample_id {0}; sample_id < 1000; ++sample_id) {
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

void ResultSet::print_results() {
    for (auto computable : computables) {
        std::cout << computable->get_name() << ": " << computable->format() << std::endl;
    }

    std::ofstream of {"out/histogram-position-resultset.csv"};
    dens.write_histogram(of);
    of.close();
}
