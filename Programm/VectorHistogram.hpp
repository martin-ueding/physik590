// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "Histogram.hpp"

#include <fstream>
#include <vector>

class VectorHistogram : public Histogram {
    public:
        VectorHistogram(size_t bins, size_t cache);

        /**
          Push a new value into the histogram.
          **/
        void push(double value);

        /**
          Writes the current histogram into the given stream.

          @param outfile Stream to write to
          */
        void write_histogram(std::ostream &outfile);

        size_t map_bin(double value);

    private:
        std::vector<double> data;
        std::vector<double> bins;
        void into_bins();
        bool has_changed {false};
        void set_bounds();
        double min;
        double max;
        bool bins_fixed {false};
        size_t cache;
        size_t points_pushed {0};

};
