// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "PositionDensity.hpp"

PositionDensity::PositionDensity(double min, double max, size_t bins)
:
    HistogramComputable{min, max, bins}
{
}

void PositionDensity::add_sample(BootstrapSample &sample) {
    FixedHistogram hist {min, max, bin_count()};
    for (size_t tr_id {0}; tr_id < sample.size(); ++tr_id) {
        sample[tr_id].binning_snapshot(hist);
    }
    insert_histogram(hist);
}

std::string PositionDensity::get_name() {
    return "Position";
}
