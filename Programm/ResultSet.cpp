// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "ResultSet.hpp"

#include <iostream>

ResultSet::ResultSet(BootstrapPool pool) {
    computables.push_back(&mean);
    computables.push_back(&moment_2);

    compute_using_pool(pool);
}

void ResultSet::compute_using_pool(BootstrapPool pool) {
    for (size_t sample_id {0}; sample_id < 10; ++sample_id) {
        BootstrapSample sample {pool};

        for (auto computable : computables) {
            computable->add_sample(sample);
        }
    }
}

void ResultSet::print_results() {
    for (auto computable : computables) {
        std::cout << computable->get_name() << ": " << computable->format() << std::endl;
    }
}
