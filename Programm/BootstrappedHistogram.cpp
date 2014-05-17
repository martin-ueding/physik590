// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "BootstrappedHistogram.hpp"

#include <fstream>

BootstrappedHistogram::BootstrappedHistogram(double min, double max, size_t bins)
    :
    min {min}, max {max},
bins {std::vector<BootstrappedQuantity>(bins)} {
}

void BootstrappedHistogram::write_histogram(std::string filename) {
    std::ofstream outfile {filename};
    double width = (max - min) / bins.size();

    for (size_t i = 0; i < bins.size(); i++) {
        double x = min + width + i * width;
        double y = bins[i].mean();
        double y_err = bins[i].stddev();

        y /= width;
        y_err /= width;

        outfile << x << "\t" << y << "\t" << y_err << std::endl;
    }
}

void BootstrappedHistogram::insert_histogram(Histogram &hist) {
    for (size_t bin {0}; bin < hist.size(); bin++) {
        bins[bin].append(hist[bin]);
    }
}
