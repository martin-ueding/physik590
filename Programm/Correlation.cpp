// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Correlation.hpp"

Correlation::Correlation(unsigned int distance) : distance{distance} {
}

void Correlation::add_sample(BootstrapSample &sample) {
    size_t count {0};
    double value {0.0};

    for (size_t trajectory_id {0}; trajectory_id < sample.size(); trajectory_id++) {
        ListQuantity &cur = sample[trajectory_id];

        value += cur.list[0] * cur.list[distance];
        count++;
    }

    append(value / count);
}

std::string Correlation::get_name() {
    return "Correlation";
}
