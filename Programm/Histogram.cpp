// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Histogram.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

Histogram::Histogram(double min, double max, size_t bins) :
    min {min},
    max {max},
bins {std::vector<int>(bins)} {
}

void Histogram::operator()(double value) {
    size_t bin = map_bin(value);
    if (bin < bins.size()) {
        bins[bin]++;
        points_pushed++;
    }
}

void Histogram::save(std::string &filename) {
    std::ofstream outfile(filename);
    double width = (max - min) / bins.size();

    for (size_t i = 0; i < bins.size(); i++) {
        double x = min + width / 2 + i * width;
        double y = (double) bins[i] / points_pushed;
        outfile << x << "\t" << y << "\t" << std::endl;
    }
}

size_t Histogram::map_bin(double value) {
    return (bins.size() - 1) * (value - min) / (max - min);
}
