// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "FixedHistogram.hpp"

#include <iostream>

FixedHistogram::FixedHistogram(double min, double max, int bins) :
    min {min},
    max {max},
    bins {std::vector<int>(bins)}
{
}

void FixedHistogram::push(double value) {
    size_t bin = map_bin(value);
    if (0 <= bin && bin < bins.size()) {
        bins[]++;
        points_pushed++;
    }
}

void FixedHistogram::write_histogram(std::ostream &outfile) {
    double width = (max - min) / bins.size();

    for (size_t i = 0; i < bins.size(); i++) {
        double x = min + width / 2 + i * width;
        double y = (double) bins[i] / points_pushed;
        outfile << x << "\t" << y << "\t" << std::endl;
    }
}

size_t FixedHistogram::map_bin(double value) {
    return (bins.size() - 1) * (value - min) / (max - min);
}
