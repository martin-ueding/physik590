// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "VectorHistogram.hpp"

#include <iostream>

VectorHistogram::VectorHistogram(size_t bins, size_t cache) : bins(std::vector<double>(bins)) {
}

void VectorHistogram::push(double value) {
    data.push_back(value);
    has_changed = true;
}

void VectorHistogram::write_histogram(std::ostream &outfile) {
    if (has_changed) {
        into_bins();
    }

    double width = (max - min) / bins.size();
    double x;
    double y;

    for (size_t i = 0; i < bins.size(); i++) {
        x = min + width/2 + i * width;
        y = (double) bins[i] / data.size();
        outfile << x << "\t" << y << std::endl;
    }
}

void VectorHistogram::into_bins() {
    min = data[0];
    max = data[0];

    // Find the limits of the histogram.
    for (double &i : data) {
        if (i < min) {
            min = i;
        }
        if (i > max) {
            max = i;
        }
    }

    size_t bin;
    for (double &i : data) {
        bin = map_bin(i);
        bins[bin]++;
    }
}

size_t VectorHistogram::map_bin(double value) {
    return (bins.size()-1) * (value - min) / (max - min);
}
