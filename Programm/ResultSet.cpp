// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "ResultSet.hpp"

#include <iostream>
#include <random>
#include <thread>

ResultSet::ResultSet(BootstrapPool &pool) : pool (pool) {
    computables.push_back(&mean);
    computables.push_back(&moment_2);

    compute_using_pool();
}

void ResultSet::operator()() {
    int sample_id = counter();
    if (sample_id % 20 == 0) {
        std::cout << "Creating BoostrapSample " << sample_id << std::endl;
    }
    BootstrapSample sample {pool, engine};

    for (auto computable : computables) {
        computable->add_sample(sample);
    }

    dens.add_sample(sample);
}

void ResultSet::compute_using_pool() {
    std::thread worker1 {this};

    worker1.join();
}

void ResultSet::print_results() {
    for (auto computable : computables) {
        std::cout << computable->get_name() << ": " << computable->format() << std::endl;
    }

    std::ofstream of {"out/histogram-position-resultset.csv"};
    dens.write_histogram(of);
    of.close();
}
