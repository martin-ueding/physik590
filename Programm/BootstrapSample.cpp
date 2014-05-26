// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapSample.hpp"

BootstrapSample::BootstrapSample(BootstrapPool &pool) :
    histogram {pool.histograms[0].get_min(), pool.histograms[0].get_max(), pool.histograms[0].size()} {
    for (auto & p : pool.even[0]) {
        Eigen::MatrixXd m {p.second.rows(), p.second.cols()};
        m.setZero();
        even.insert(CorrFunc::value_type {p.first, m});
    }

    for (auto & p : pool.odd[0]) {
        Eigen::MatrixXd m {p.second.rows(), p.second.cols()};
        m.setZero();
        odd.insert(CorrFunc::value_type {p.first, m});
    }

    std::uniform_int_distribution<size_t> dist {0, pool.size() - 1};

    for (unsigned index_id {0}; index_id < pool.size(); index_id++) {
        unsigned i = dist(pool.engine);

        auto &even_element = pool.even[i];
        for (auto & p : even_element) {
            even[p.first] += p.second;
        }

        auto &odd_element = pool.odd[i];
        for (auto & p : odd_element) {
            odd[p.first] += p.second;
        }

        histogram += pool.histograms[i];
    }

    // Normalize all correlation matrices.
    for (auto &pair : even) {
        pair.second /= pool.size();
    }

    // Normalize all correlation matrices.
    for (auto &pair : odd) {
        pair.second /= pool.size();
    }
}
