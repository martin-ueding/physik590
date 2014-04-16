// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#pragma once

#include "Computable.hpp"
#include "BootstrappedHistogram.hpp"

class HistogramComputable : public Computable, public BootstrappedHistogram {
    public:
        HistogramComputable(double min, double max, size_t bins);
};
