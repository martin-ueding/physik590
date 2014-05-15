// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "PositionDensity.hpp"

PositionDensity::PositionDensity(double min, double max, size_t bins)
    :
    HistogramComputable {min, max, bins} {
}

void PositionDensity::add_sample(BootstrapSample &sample) {
    insert_histogram(sample.histogram);
}

std::string PositionDensity::get_name() {
    return "Position";
}
