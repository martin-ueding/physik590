// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrappedHistogram.hpp"

#include <ostream>

BootstrappedHistogram::BootstrappedHistogram(double min, double max, size_t bins)
    :
    min {min}, max {max},
bins {std::vector<BootstrappedQuantity>(bins)} {
}

void BootstrappedHistogram::write_histogram(std::ostream &outfile) {
    double width = (max - min) / bins.size();

    for (size_t i = 0; i < bins.size(); i++) {
        double x = min + width + i * width;
        auto y_and_err = bins[i].mean_and_stddev();
        double y = y_and_err.first;
        double y_err = y_and_err.second;

        y /= width;
        y_err /= width;

        outfile << x << "\t" << y_and_err.first << "\t" << y_and_err.second << std::endl;
    }
}

void BootstrappedHistogram::insert_histogram(FixedHistogram &hist) {
    for (size_t bin {0}; bin < hist.size(); bin++) {
        bins[bin].append(hist[bin]);
    }
}
