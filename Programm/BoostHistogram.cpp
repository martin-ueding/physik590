// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BoostHistogram.hpp"

#include "SizePrinter.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

using namespace boost::accumulators;

BoostHistogram::BoostHistogram(int bins, size_t cache) : acc(accumulator_set<double, features<tag::min, tag::max, tag::mean, tag::density>>(tag::density::num_bins = bins, tag::density::cache_size = std::min(cache, MAX_CACHE_ENTRIES))) {
    SizePrinter sp;
    if (cache > MAX_CACHE_ENTRIES) {
        std::cout << "Warning: " << sp.format(cache * sizeof(double))
                  << " is exeeding "
                  << sp.format(MAX_CACHE_ENTRIES * sizeof(double))
                  << std::endl;
    }
}

void BoostHistogram::write_histogram(std::ostream &outfile) {
    auto hist = density(acc);
    for (unsigned int i = 0; i < hist.size(); i++) {
        outfile << hist[i].first << "\t" << hist[i].second << std::endl;
    }
    outfile << "# Min " << min(acc) << std::endl;
    outfile << "# Max " << max(acc) << std::endl;
    outfile << "# Mean " << mean(acc) << std::endl;
}

void BoostHistogram::push(double value) {
    acc(value);
}
