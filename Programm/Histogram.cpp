// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Histogram.hpp"

#include <iostream>
#include <fstream>


Histogram::Histogram(int bins) : acc(accumulator_set<double, features<tag::density>>(tag::density::num_bins = bins, tag::density::cache_size = 100)) {
}

void Histogram::print() {
            auto hist = density(acc);
            for (unsigned int i = 0; i < hist.size(); i++) {
                std::cout << hist[i].first << "\t" << hist[i].second << std::endl;
            }
        }

void Histogram::save(std::string filename) {
}
