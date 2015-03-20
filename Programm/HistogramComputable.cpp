// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "HistogramComputable.hpp"

HistogramComputable::HistogramComputable(double min, double max, size_t bins)
    : BootstrappedHistogram{min, max, bins} {
}
