// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "VectorHistogram.hpp"

VectorHistogram::VectorHistogram(size_t bins) : data(std::vector<double>(bins)) {
}

void VectorHistogram::push(double value) {
    data.push_back(value);
    has_changed = true;
}

void VectorHistogram::write_histogram(std::ostream &outfile) {
    if (has_changed) {
        into_bins();
    }

    for (size_t i = 0; i < bins.size(); i++) {
        outfile << i << "\t" << bins[i] << std::endl;
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

    for (double &i : data) {
        bins[map_bin(i)]++;
    }
}

size_t VectorHistogram::map_bin(double value) {
    return bins.size() * (value - min) / (max - min);
}
