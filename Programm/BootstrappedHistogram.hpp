// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "BootstrappedQuantity.hpp"
#include "FixedHistogram.hpp"

class BootstrappedHistogram {
    public:
        BootstrappedHistogram(double min, double max, int bins);

        void push(double value);

        void write_histogram(std::ostream &outfile);

    protected:
        FixedHistogram hist;
        std::vector<BootstrappedQuantity> bins;
};
