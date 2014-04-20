// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "VectorHistogram.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>

VectorHistogram::VectorHistogram(size_t bins, size_t cache) :
    bins(std::vector<double>(bins)),
    cache(std::min(cache, MAX_CACHE_ENTRIES)) {
}

void VectorHistogram::push(double value) {
    if (bins_fixed) {
        unsigned int bin = map_bin(value);
        if (bin < bins.size()) {
            bins[bin]++;
        }
    }
    else {
        data.push_back(value);
        has_changed = true;

        if (data.size() >= cache) {
            std::cout << "Binning …" << std::endl;
            set_bounds();
            into_bins();
        }
    }
    points_pushed++;
}

void VectorHistogram::write_histogram(std::ostream &outfile) {
    if (has_changed) {
        into_bins();
    }

    outfile << "# Points: " << points_pushed << std::endl;

    double width = (max - min) / bins.size();
    double x;
    double y;
    double err;

    for (size_t i = 0; i < bins.size(); i++) {
        x = min + width / 2 + i * width;
        y = (double) bins[i] / points_pushed;
        err = std::sqrt(bins[i]) / points_pushed;
        outfile << x << "\t" << y << "\t" << err << std::endl;
    }
}

void VectorHistogram::set_bounds() {
    if (bins_fixed) {
        throw std::runtime_error("Bounds can only be set once!");
    }

    min = data[0];
    max = data[0];

    // Find the limits of the histogram.
    for (double & i : data) {
        if (i < min) {
            min = i;
        }
        if (i > max) {
            max = i;
        }
    }

    bins_fixed = true;
}

void VectorHistogram::into_bins() {
    if (!bins_fixed) {
        set_bounds();
    }

    size_t bin;
    double i;
    while (!data.empty()) {
        i = data.back();
        data.pop_back();
        bin = map_bin(i);
        bins[bin]++;
    }
    data.shrink_to_fit();
}

size_t VectorHistogram::map_bin(double value) {
    return (bins.size() - 1) * (value - min) / (max - min);
}
