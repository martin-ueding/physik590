// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Histogram.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>


Histogram::Histogram(int bins, int cache) : acc(accumulator_set<double, features<tag::density>>(tag::density::num_bins = bins, tag::density::cache_size = std::min(cache, MAX_CACHE))) {
    if (cache > MAX_CACHE) {
        std::cout << "Warning: " << cache << " is exeeding " << MAX_CACHE << std::endl;
    }
}

void Histogram::print() {
    write_histogram(std::cout);
}

void Histogram::save(std::string filename) {
    std::ofstream outfile(filename);
    write_histogram(outfile);
    outfile.close();
}

void Histogram::write_histogram(std::ostream &outfile) {
    auto hist = density(acc);
    for (unsigned int i = 0; i < hist.size(); i++) {
        outfile << hist[i].first << "\t" << hist[i].second << std::endl;
    }
}
