// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrappedHistogram.hpp"

#include <ostream>

BootstrappedHistogram::BootstrappedHistogram(double min, double max, int bins)
    :
        min {min}, max{max},
    bins {std::vector<BootstrappedQuantity>(bins)} {
}

void BootstrappedHistogram::write_histogram(std::ostream &outfile) {
    double width = (max - min) / bins.size();

    for (size_t i = 0; i < bins.size(); i++) {
        double x = min + width / 2 + i * width;
        auto y_and_err = bins[i].mean_and_stddev();
        outfile << x << "\t" << y_and_err.first << "\t" << y_and_err.second << std::endl;
    }
}

void BootstrappedHistogram::insert_histogram(FixedHistogram &hist) {
}
