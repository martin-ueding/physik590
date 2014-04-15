// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "ResultSet.hpp"

ResultSet::ResultSet(BootstrapPool pool) {
    computables.push_back(&mean);

    compute_using_pool(pool);
}

void ResultSet::compute_using_pool(BootstrapPool pool) {
    for (size_t sample_id {0}; sample_id < 100; ++sample_id) {
        BootstrapSample sample {pool};

        for (auto computable : computables) {
            computable->add_sample(sample);
        }
    }
}
