// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrapSample.hpp"

BootstrapSample::BootstrapSample(BootstrapPool &pool, std::mt19937 &engine) {
    for (auto &p : pool.even) {
        boost::numeric::ublas::matrix<double> m {p.second.size1, p.second.size2};
        m.clear();
        even.insert(even.value_type{p.first, m});
    }

    for (auto &p : pool.odd) {
        boost::numeric::ublas::matrix<double> m {p.second.size1, p.second.size2};
        m.clear();
        odd.insert(even.value_type{p.first, m});
    }

    std::uniform_int_distribution<unsigned> dist {0, pool.size() - 1};

    for (unsigned index_id {0}; index_id < pool.size(); index_id++) {
        unsigned i = dist(engine);

        auto &even_element = pool.even[i];
        for (auto &p : even_element) {
            even[p.first] += p.second;
        }

        auto &odd_element = pool.odd[i];
        for (auto &p : odd_element) {
            odd[p.first] += p.second;
        }

        Histogram += pool.histograms[i];
    }
}
