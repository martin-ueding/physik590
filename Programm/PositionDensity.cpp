// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "PositionDensity.hpp"

void PositionDensity::add_sample(BootstrapSample &sample) {
    for (size_t tr_id {0}; tr_id < sample.size(); ++tr_id) {
        FixedHistogram hist {min, max, bin_count()};
        sample[tr_id].binning_snapshot(hist);
        insert_histogram(hist);
    }
}

std::string PositionDensity::get_name() {
    return "Position";
}
