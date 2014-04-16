// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "Histogram.hpp"

#include <vector>

class FixedHistogram : public Histogram {
    public:
        FixedHistogram(double min, double max, size_t bins);

        void push(double value);

        void write_histogram(std::ostream &outfile);

        double get_min() {
            return min;
        }

        double get_max() {
            return max;
        }

        double size() {
            return bins.size();
        }

        double operator[](size_t i) {
            return static_cast<double>(bins[i]) / points_pushed;
        }

    protected:
        double min;
        double max;
        std::vector<int> bins;
        int points_pushed {0};

        size_t map_bin(double value);
};