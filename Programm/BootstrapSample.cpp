// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapSample.hpp"

BootstrapSample::BootstrapSample(BootstrapPool &pool, Settings &settings) : histogram {pool.histograms[0].get_min(), pool.histograms[0].get_max(), pool.histograms[0].size()} {

    Eigen::MatrixXd m {settings.correlation_size, settings.correlation_size};
    m.setZero();
    for (unsigned i = 0; i != settings.correlation_ts.size(); i++) {
        even.push_back(m);
        odd.push_back(m);
    }

    std::uniform_int_distribution<size_t> dist {settings.bootstrap_min, pool.size() - 1};
    unsigned count = pool.size() - settings.bootstrap_min;

    for (unsigned index_id {0}; index_id < count; index_id++) {
        unsigned i = dist(pool.engine);

        for (unsigned j = 0; j != settings.correlation_ts.size(); j++) {
            even[j] += pool.even[i][j];
        }

        for (unsigned j = 0; j != settings.correlation_ts.size(); j++) {
            odd[j] += pool.odd[i][j];
        }

        histogram += pool.histograms[i];

        e0_virial += pool.e0_virial[i];
    }

    e0_virial += count;

    // Normalize all correlation matrices.
    for (auto &matrix : even) {
        matrix /= count;
    }

    // Normalize all correlation matrices.
    for (auto &matrix : odd) {
        matrix /= count;
    }
}
